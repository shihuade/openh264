#include "wels_common_basis.h"
#include "utils/BufferedData.h"
#include "BaseEncoderTest.h"
#include "svc_encode_slice.h"
#include "encoder.h"
#include "macros.h"
#include "EncUT_SliceBufferReallocate.h"

namespace WelsEnc {
	extern void FreeDqLayer(SDqLayer*& pDq, CMemoryAlign* pMa);
	extern void FreeMemorySvc(sWelsEncCtx** ppCtx);
	extern int32_t AcquireLayersNals(sWelsEncCtx** ppCtx,
		                               SWelsSvcCodingParam* pParam,
		                               int32_t* pCountLayers,
		                               int32_t* pCountNals);
}

void CSliceBufferReallocatTest::EncodeStream(InputStream* in, SEncParamExt* pEncParamExt) {
	ASSERT_TRUE(NULL != pEncParamExt);
	int frameSize = pEncParamExt->iPicWidth * pEncParamExt->iPicHeight * 3 / 2;

	BufferedData buf;
	buf.SetLength(frameSize);
	ASSERT_TRUE(buf.Length() == (size_t)frameSize);

	SFrameBSInfo info;
	memset(&info, 0, sizeof(SFrameBSInfo));

	SSourcePicture pic;
	memset(&pic, 0, sizeof(SSourcePicture));
	pic.iPicWidth = pEncParamExt->iPicWidth;
	pic.iPicHeight = pEncParamExt->iPicHeight;
	pic.iColorFormat = videoFormatI420;
	pic.iStride[0] = pic.iPicWidth;
	pic.iStride[1] = pic.iStride[2] = pic.iPicWidth >> 1;
	pic.pData[0] = buf.data();
	pic.pData[1] = pic.pData[0] + pEncParamExt->iPicWidth * pEncParamExt->iPicHeight;
	pic.pData[2] = pic.pData[1] + (pEncParamExt->iPicWidth * pEncParamExt->iPicHeight >> 2);

	int iRet = InitWithParam(m_pEncoder, pEncParamExt);
	ASSERT_TRUE(iRet == cmResultSuccess);

	while (in->read(buf.data(), frameSize) == frameSize) {
		int32_t iRet = m_pEncoder->EncodeFrame(&pic, &info);
		ASSERT_TRUE(iRet == cmResultSuccess);
	}
}

void CSliceBufferReallocatTest::InitParam() {
	sWelsEncCtx* pCtx = &m_EncContext;
	SWelsFuncPtrList sEncFunctionList;
	pCtx->pFuncList = &sEncFunctionList;

	pCtx->pSvcParam->iMultipleThreadIdc = (rand() % MAX_THREADS_NUM) + 1;
	pCtx->iActiveThreadsNum = pCtx->pSvcParam->iMultipleThreadIdc;
	pCtx->pSvcParam->iSpatialLayerNum = 1;
	pCtx->pSvcParam->bSimulcastAVC = (bool)rand() % 2;

	pCtx->pSvcParam->iPicHeight = (((rand() % MAX_WIDTH ) >> 4 ) << 4) + 16;
	pCtx->pSvcParam->iPicWidth  = (((rand() % MAX_HEIGH ) >> 4) << 4) + 16;
	pCtx->iGlobalQp = WelsClip3( rand() % 35, 26, 35);
	pCtx->pSvcParam->iRCMode = RC_OFF_MODE;
	pCtx->pSvcParam->iTargetBitrate = WelsClip3(rand() % MAX_BIT_RATE, MIN_BIT_RATE, MAX_BIT_RATE);
	int32_t iParamStraIdx = rand() % 5;
	pCtx->pSvcParam->eSpsPpsIdStrategy = (EParameterSetStrategy)(iParamStraIdx == 4 ? 0x06 : iParamStraIdx);

	pCtx->pFuncList = (SWelsFuncPtrList*)pCtx->pMemAlign->WelsMallocz(sizeof(SWelsFuncPtrList), "SWelsFuncPtrList");
	ASSERT_TRUE(NULL != pCtx->pFuncList);

	pCtx->pFuncList->pParametersetStrategy = IWelsParametersetStrategy::CreateParametersetStrategy(pCtx->pSvcParam->eSpsPpsIdStrategy,
		pCtx->pSvcParam->bSimulcastAVC, pCtx->pSvcParam->iSpatialLayerNum);
	ASSERT_TRUE(NULL != pCtx->pFuncList->pParametersetStrategy);

	pCtx->ppDqLayerList = (SDqLayer**)pCtx->pMemAlign->WelsMallocz(pCtx->pSvcParam->iSpatialLayerNum * sizeof(SDqLayer*), "ppDqLayerList");
	ASSERT_TRUE(NULL != pCtx->ppDqLayerList);
}

void CSliceBufferReallocatTest::UnInitParam(){
	sWelsEncCtx* pCtx = &m_EncContext;
	if (NULL != pCtx->pFuncList->pParametersetStrategy) {
		delete pCtx->pFuncList->pParametersetStrategy;
		pCtx->pFuncList->pParametersetStrategy = NULL;
	}

	if (NULL != pCtx->pFuncList) {
		pCtx->pMemAlign->WelsFree(pCtx->pFuncList, "pCtx->pFuncList");
		pCtx->pFuncList = NULL;
	}

	if (NULL != pCtx->ppDqLayerList) {
		pCtx->pMemAlign->WelsFree(pCtx->ppDqLayerList, "pCtx->ppDqLayerList");
		pCtx->ppDqLayerList = NULL;
	}
}

void CSliceBufferReallocatTest::InitFrameBsBuffer() {
	const int32_t iLayerIdx = 0;
	sWelsEncCtx* pCtx = &m_EncContext;
	SSpatialLayerConfig* pLayerCfg = &pCtx->pSvcParam->sSpatialLayers[iLayerIdx];
	const int32_t kiSpsSize = pCtx->pFuncList->pParametersetStrategy->GetNeededSpsNum() * SPS_BUFFER_SIZE;
	const int32_t kiPpsSize = pCtx->pFuncList->pParametersetStrategy->GetNeededPpsNum() * PPS_BUFFER_SIZE;

	int32_t iNonVclLayersBsSizeCount = SSEI_BUFFER_SIZE + kiSpsSize + kiPpsSize;
	int32_t iLayerBsSize = WELS_ROUND(((3 * pLayerCfg->iVideoWidth * pLayerCfg->iVideoHeight) >> 1) * COMPRESS_RATIO_THR) + MAX_MACROBLOCK_SIZE_IN_BYTE_x2;
	int32_t iVclLayersBsSizeCount = WELS_ALIGN(iLayerBsSize, 4);
	int32_t iCountBsLen = iNonVclLayersBsSizeCount + iVclLayersBsSizeCount;
	int32_t iCountNals = 0;

	int32_t iRet = AcquireLayersNals(&pCtx, pCtx->pSvcParam, &pCtx->pSvcParam->iSpatialLayerNum, &iCountNals);
	ASSERT_TRUE(0 == iRet);

	// Output
	pCtx->pOut = (SWelsEncoderOutput*)pCtx->pMemAlign->WelsMallocz(sizeof(SWelsEncoderOutput), "SWelsEncoderOutput");
	ASSERT_TRUE(NULL != pCtx->pOut);
	pCtx->pOut->pBsBuffer = (uint8_t*)pCtx->pMemAlign->WelsMallocz(iCountBsLen, "pOut->pBsBuffer");
	ASSERT_TRUE(NULL != pCtx->pOut->pBsBuffer);
	pCtx->pOut->uiSize = iCountBsLen;
	pCtx->pOut->sNalList = (SWelsNalRaw*)pCtx->pMemAlign->WelsMallocz(iCountNals * sizeof(SWelsNalRaw), "pOut->sNalList");
	ASSERT_TRUE(NULL != pCtx->pOut->sNalList);
	pCtx->pOut->pNalLen = (int32_t*)pCtx->pMemAlign->WelsMallocz(iCountNals * sizeof(int32_t), "pOut->pNalLen");
	ASSERT_TRUE(NULL != pCtx->pOut->pNalLen);
	pCtx->pOut->iCountNals = iCountNals;
	pCtx->pOut->iNalIndex = 0;
	pCtx->pOut->iLayerBsIndex = 0;
	pCtx->pFrameBs = (uint8_t*)pCtx->pMemAlign->WelsMalloc(iCountBsLen, "pFrameBs");
	ASSERT_TRUE(NULL != pCtx->pOut);
	pCtx->iFrameBsSize = iCountBsLen;
	pCtx->iPosBsBuffer = 0;
}

void CSliceBufferReallocatTest::UnInitFrameBsBuffer() {
	sWelsEncCtx* pCtx = &m_EncContext;

	if (NULL != pCtx->pOut->pBsBuffer) {
		pCtx->pMemAlign->WelsFree(pCtx->pOut->pBsBuffer, "pCtx->pOut->pBsBuffer");
		pCtx->pOut->pBsBuffer = NULL;
	}

	if (NULL != pCtx->pOut->sNalList) {
		pCtx->pMemAlign->WelsFree(pCtx->pOut->sNalList, "pCtx->pOut->sNalList");
		pCtx->pOut->sNalList = NULL;
	}

	if (NULL != pCtx->pOut->pNalLen) {
		pCtx->pMemAlign->WelsFree(pCtx->pOut->pNalLen, "pCtx->pOut->pNalLen");
		pCtx->pOut->pNalLen = NULL;
	}

	if (NULL != pCtx->pOut) {
		pCtx->pMemAlign->WelsFree(pCtx->pOut, "pCtx->pOut");
		pCtx->pOut = NULL;
	}

	if (NULL != pCtx->pFrameBs) {
		pCtx->pMemAlign->WelsFree(pCtx->pFrameBs, "pCtx->pFrameBs");
		pCtx->pFrameBs = NULL;
	}
}

int32_t AllocateLayerBuffer(sWelsEncCtx* pCtx, const int32_t iLayerIdx) {
	SSpatialLayerConfig* pLayerCfg = &pCtx->pSvcParam->sSpatialLayers[iLayerIdx];
	SDqLayer* pDqLayer = (SDqLayer*)pCtx->pMemAlign->WelsMallocz(sizeof(SDqLayer), "pDqLayer");
	if (NULL == pDqLayer) {
		return ENC_RETURN_MEMALLOCERR;
	}

	pDqLayer->iMbWidth = (pLayerCfg->iVideoWidth + 15) >> 4;
	pDqLayer->iMbHeight = (pLayerCfg->iVideoHeight + 15) >> 4;
	pDqLayer->iMaxSliceNum = GetInitialSliceNum(&pLayerCfg->sSliceArgument);

	int32_t iRet = InitSliceInLayer(pCtx, pDqLayer, iLayerIdx, pCtx->pMemAlign);
	if (ENC_RETURN_SUCCESS != iRet) {
		FreeDqLayer(pDqLayer, pCtx->pMemAlign);
		return ENC_RETURN_MEMALLOCERR;
	}

	pCtx->ppDqLayerList[iLayerIdx] = pDqLayer;
	return ENC_RETURN_SUCCESS;
}

void SetPartitonMBNum(SDqLayer* pCurDqLayer, SSpatialLayerConfig* pLayerCfg, int32_t iPartNum) {
	int32_t iMBWidth = (pLayerCfg->iVideoWidth + 15) >> 4;
	int32_t iMBHeight = (pLayerCfg->iVideoHeight + 15) >> 4;
	int32_t iMbNumInFrame = iMBWidth * iMBHeight;
	int32_t iMBPerPart = iMbNumInFrame / iPartNum;

	if (0 == iMBPerPart) {
		iPartNum = 1;
		iMBPerPart = iMbNumInFrame;
	}

	for (int32_t iPartIdx = 0; iPartIdx < (iPartNum - 1); iPartIdx++) {
		pCurDqLayer->FirstMbIdxOfPartition[iPartIdx] = iMBPerPart * iPartIdx;
		pCurDqLayer->EndMbIdxOfPartition[iPartIdx] = pCurDqLayer->FirstMbIdxOfPartition[iPartIdx] + iMBPerPart - 1;
	}

	pCurDqLayer->FirstMbIdxOfPartition[iPartNum - 1] = iMBPerPart * (iPartNum - 1);
	pCurDqLayer->EndMbIdxOfPartition[iPartNum - 1] = iMbNumInFrame - 1;

	for (int32_t iPartIdx = iPartNum; iPartIdx < MAX_THREADS_NUM; iPartIdx++) {
		pCurDqLayer->FirstMbIdxOfPartition[iPartIdx] = 0;
		pCurDqLayer->EndMbIdxOfPartition[iPartIdx] = 0;
	}
}

int32_t IntParamForSizeLimitSlcMode(sWelsEncCtx* pCtx, const int32_t iLayerIdx) {
	SSpatialLayerConfig* pLayerCfg = &pCtx->pSvcParam->sSpatialLayers[iLayerIdx];
	SSliceArgument* pSliceArgument = &pLayerCfg->sSliceArgument;
	int32_t iSliceBufferSize       = 0;
	int32_t iLayerBsSize = WELS_ROUND(((3 * pLayerCfg->iVideoWidth * pLayerCfg->iVideoHeight) >> 1) * COMPRESS_RATIO_THR)
		                   + MAX_MACROBLOCK_SIZE_IN_BYTE_x2;
	pSliceArgument->uiSliceSizeConstraint = 600;
	pCtx->pSvcParam->uiMaxNalSize = 1500;

	int32_t iMaxSliceNumEstimation = WELS_MIN(AVERSLICENUM_CONSTRAINT,
		                                        (iLayerBsSize / pSliceArgument->uiSliceSizeConstraint) + 1);
	pCtx->iMaxSliceCount = WELS_MAX(pCtx->iMaxSliceCount, iMaxSliceNumEstimation);
	iSliceBufferSize     = (WELS_MAX(pSliceArgument->uiSliceSizeConstraint,
		                      iLayerBsSize / iMaxSliceNumEstimation) << 1) + MAX_MACROBLOCK_SIZE_IN_BYTE_x2;
	pCtx->iSliceBufferSize[iLayerIdx] = iSliceBufferSize;

	int32_t iRet = AllocateLayerBuffer(pCtx, iLayerIdx);
	if (ENC_RETURN_SUCCESS != iRet) {
		return ENC_RETURN_MEMALLOCERR;
	}

	SetPartitonMBNum(pCtx->ppDqLayerList[iLayerIdx], pLayerCfg, pCtx->iActiveThreadsNum);
	return ENC_RETURN_SUCCESS;
}

void IntParamForRasterSlcMode(sWelsEncCtx* pCtx, const int32_t iLayerIdx) {
	SSpatialLayerConfig* pLayerCfg = &pCtx->pSvcParam->sSpatialLayers[iLayerIdx];
	SSliceArgument* pSliceArgument = &pLayerCfg->sSliceArgument;
	int32_t iMBWidth = (pLayerCfg->iVideoWidth + 15) >> 4;
	int32_t iMBHeight = (pLayerCfg->iVideoHeight + 15) >> 4;
	int32_t iMbNumInFrame = iMBWidth * iMBHeight;
	int32_t iSliceMBNum = 0;

	pSliceArgument->uiSliceMbNum[0] = rand() % 2;
	if (0 == pSliceArgument->uiSliceMbNum[0] && iMBHeight > MAX_SLICES_NUM) {
		pSliceArgument->uiSliceNum = MAX_SLICES_NUM;
		pSliceArgument->uiSliceMbNum[0] = 1;
	}

	if (0 != pSliceArgument->uiSliceMbNum[0]) {
		iSliceMBNum = iMbNumInFrame / pSliceArgument->uiSliceNum;
		for (int32_t iSlcIdx = 0; iSlcIdx < pSliceArgument->uiSliceNum - 1; iSlcIdx++) {
			pSliceArgument->uiSliceMbNum[iSlcIdx] = iSliceMBNum;
		}
		iSliceMBNum = iMbNumInFrame / pSliceArgument->uiSliceNum;
		pSliceArgument->uiSliceMbNum[pSliceArgument->uiSliceNum - 1] = iMbNumInFrame - iSliceMBNum * (pSliceArgument->uiSliceNum - 1);
	}
}

void CSliceBufferReallocatTest::InitLayerSliceBuffer(const int32_t iLayerIdx) {
	sWelsEncCtx* pCtx = &m_EncContext;
  SSpatialLayerConfig* pLayerCfg = &pCtx->pSvcParam->sSpatialLayers[iLayerIdx];
	SSliceArgument* pSliceArgument = &pLayerCfg->sSliceArgument;
	int32_t iLayerBsSize = 0;
	int32_t iSliceBufferSize = 0;
	int32_t iRet = 0;

	pLayerCfg->iVideoWidth = pCtx->pSvcParam->iPicWidth >> (pCtx->pSvcParam->iSpatialLayerNum -1 - iLayerIdx);
	pLayerCfg->iVideoHeight = pCtx->pSvcParam->iPicHeight >> (pCtx->pSvcParam->iSpatialLayerNum - 1 - iLayerIdx);
	pLayerCfg->iSpatialBitrate = pCtx->pSvcParam->iTargetBitrate / pCtx->pSvcParam->iSpatialLayerNum;

	//Slice argument
	pSliceArgument->uiSliceMode = (SliceModeEnum)(rand() % 4);
	pSliceArgument->uiSliceNum = rand() % MAX_SLICES_NUM + 1;

	if (pSliceArgument->uiSliceMode == SM_SIZELIMITED_SLICE) {
		iRet = IntParamForSizeLimitSlcMode(pCtx, iLayerIdx);
	} else {
		if (pSliceArgument->uiSliceMode == SM_RASTER_SLICE) {
			IntParamForRasterSlcMode(pCtx, iLayerIdx);
		}

		iLayerBsSize = WELS_ROUND(((3 * pLayerCfg->iVideoWidth * pLayerCfg->iVideoHeight) >> 1) * COMPRESS_RATIO_THR)
			                       + MAX_MACROBLOCK_SIZE_IN_BYTE_x2;
		pCtx->iMaxSliceCount = WELS_MAX(pCtx->iMaxSliceCount, (int)pSliceArgument->uiSliceNum);
		iSliceBufferSize = ((iLayerBsSize / pSliceArgument->uiSliceNum) << 1) + MAX_MACROBLOCK_SIZE_IN_BYTE_x2;

		pCtx->iSliceBufferSize[iLayerIdx] = iSliceBufferSize;
		iRet = AllocateLayerBuffer(pCtx, iLayerIdx);
	}

	ASSERT_TRUE(ENC_RETURN_SUCCESS == iRet);
	ASSERT_TRUE(NULL != pCtx->ppDqLayerList[iLayerIdx]);

	pCtx->uiDependencyId = iLayerIdx;
	pCtx->pCurDqLayer = pCtx->ppDqLayerList[iLayerIdx];
}

void CSliceBufferReallocatTest::UnInitLayerSliceBuffer(const int32_t iLayerIdx) {
	sWelsEncCtx* pCtx = &m_EncContext;
	if (NULL != pCtx->ppDqLayerList[iLayerIdx]) {
		FreeDqLayer(pCtx->ppDqLayerList[iLayerIdx], pCtx->pMemAlign);
		pCtx->ppDqLayerList[iLayerIdx] = NULL;
	}
}
/*
TEST_F(CSliceBufferReallocatTest, ReallocateTest) {
	sWelsEncCtx* pCtx = &m_EncContext;
	EncodeFileParam pEncFileParam; // = GetParam();
	pEncFileParam.pkcFileName = "res/CiscoVT2people_320x192_12fps.yuv";
	pEncFileParam.iWidth      = 320;
	pEncFileParam.iHeight     = 192;
	pEncFileParam.fFrameRate  = 12.0;
	FileInputStream fileStream;
	ASSERT_TRUE(fileStream.Open(pEncFileParam.pkcFileName));
	m_EncContext.pSvcParam->iUsageType    = CAMERA_VIDEO_REAL_TIME;
	m_EncContext.pSvcParam->iPicHeight    = pEncFileParam.iHeight;
	m_EncContext.pSvcParam->iPicWidth     = pEncFileParam.iWidth;
	m_EncContext.pSvcParam->fMaxFrameRate = pEncFileParam.fFrameRate;
	EncodeStream(&fileStream, m_EncContext.pSvcParam);

	FreeMemorySvc(&pCtx);
}
*/

int32_t RandAvailableThread(sWelsEncCtx* pCtx, const int32_t kiMinBufferNum) {
	int32_t iLayerIdx = 0;
	int32_t aiThrdList[MAX_THREADS_NUM] = { -1 };
	int32_t iCodedSlcNum = 0;
	int32_t iMaxSlcNumInThrd = 0;
	int32_t iAvailableThrdNum = 0;
	int32_t iRandThrdIdx = -1;

	if (NULL == pCtx || NULL == pCtx->pCurDqLayer || pCtx->iActiveThreadsNum <= 0) {
		return -1;
	}

	for (int32_t iThrdIdx = 0; iThrdIdx < pCtx->iActiveThreadsNum; iThrdIdx++) {
		iCodedSlcNum = pCtx->pCurDqLayer->sSliceThreadInfo[iThrdIdx].iCodedSliceNum;
		iMaxSlcNumInThrd = pCtx->pCurDqLayer->sSliceThreadInfo[iThrdIdx].iMaxSliceNum;

		if ((iCodedSlcNum + kiMinBufferNum) <= iMaxSlcNumInThrd) {
			aiThrdList[iAvailableThrdNum] = iThrdIdx;
			iAvailableThrdNum++;
		}
	}

	if (0 == iAvailableThrdNum) {
		return -1;
	}
	iRandThrdIdx = rand() % iAvailableThrdNum;

	return aiThrdList[iRandThrdIdx];
}

void CSliceBufferReallocatTest::SimulateEncodedOneSlice(const int32_t kiSlcIdx, const int32_t kiThreadIdx) {
	int32_t iCodedSlcNumInThrd = m_EncContext.pCurDqLayer->sSliceThreadInfo[kiThreadIdx].iCodedSliceNum;

	ASSERT_TRUE(NULL != m_EncContext.pCurDqLayer->sSliceThreadInfo[kiThreadIdx].pSliceInThread);
	ASSERT_TRUE(NULL != &m_EncContext.pCurDqLayer->sSliceThreadInfo[kiThreadIdx].pSliceInThread[iCodedSlcNumInThrd]);

	m_EncContext.pCurDqLayer->sSliceThreadInfo[kiThreadIdx].pSliceInThread[iCodedSlcNumInThrd].iSliceIdx = kiSlcIdx;
	m_EncContext.pCurDqLayer->sSliceThreadInfo[kiThreadIdx].iCodedSliceNum ++;
}

void CSliceBufferReallocatTest::SimulateSliceInOnePartition(const int32_t kiPartNum,
	                                                          const int32_t kiPartIdx,
																														const int32_t kiSlcNumInPart) {
	int32_t iSlcIdxInPart = 0;

	//slice within same partition will encoded by same thread in current design
	int32_t iPartitionThrdIdx = RandAvailableThread(&m_EncContext, kiSlcNumInPart);
	ASSERT_TRUE(-1 != iPartitionThrdIdx);

	for (int32_t iSlcIdx = 0; iSlcIdx < kiSlcNumInPart; iSlcIdx++) {
		iSlcIdxInPart = kiPartIdx + kiPartNum * iSlcIdx;

		SimulateEncodedOneSlice(iSlcIdxInPart, iPartitionThrdIdx);

		m_EncContext.pCurDqLayer->NumSliceCodedOfPartition[kiPartIdx] ++;
		m_EncContext.pCurDqLayer->sSliceEncCtx.iSliceNumInFrame ++;
	}
}

void CSliceBufferReallocatTest::SimulateSliceInOneLayer() {
	int32_t iLayerIdx = 0;
	SSpatialLayerConfig* pLayerCfg = &m_EncContext.pSvcParam->sSpatialLayers[iLayerIdx];
	int32_t iTotalSliceBuffer = m_EncContext.pCurDqLayer->iMaxSliceNum;
	int32_t iSimulateSliceNum = rand() % iTotalSliceBuffer + 1;

	if (SM_SIZELIMITED_SLICE == pLayerCfg->sSliceArgument.uiSliceMode) {
		int32_t iPartNum         = m_EncContext.iActiveThreadsNum;
		int32_t iSlicNumPerPart  = iSimulateSliceNum / iPartNum;
		int32_t iMaxSlcNumInThrd = m_EncContext.pCurDqLayer->sSliceThreadInfo[0].iMaxSliceNum;
		int32_t iLastPartSlcNum = 0;

		iSlicNumPerPart = WelsClip3(iSlicNumPerPart, 1, iMaxSlcNumInThrd);
		iLastPartSlcNum = iSimulateSliceNum - iSlicNumPerPart * (iPartNum - 1);
		iLastPartSlcNum = WelsClip3(iLastPartSlcNum, 1, iMaxSlcNumInThrd);

		for (int32_t iPartIdx = 0; iPartIdx < iPartNum; iPartIdx ++) {
			int32_t iSlcNumInPart = (iPartIdx < (iPartNum - 1)) ? iSlicNumPerPart : iLastPartSlcNum;
			SimulateSliceInOnePartition(iPartNum, iPartIdx, iSlcNumInPart);
		}
	} else {
		for (int32_t iSlcIdx = 0; iSlcIdx < iSimulateSliceNum; iSlcIdx ++) {
			int32_t iSlcThrdIdx = RandAvailableThread(&m_EncContext, 1);
			ASSERT_TRUE(-1 != iSlcThrdIdx);

			SimulateEncodedOneSlice(iSlcIdx, iSlcThrdIdx);
			m_EncContext.pCurDqLayer->sSliceEncCtx.iSliceNumInFrame++;
		}
	}
}

TEST_F(CSliceBufferReallocatTest, ReorderTest) {
	int32_t iLayerIdx = 0;
	sWelsEncCtx* pCtx = &m_EncContext;
	SSpatialLayerConfig* pLayerCfg = &pCtx->pSvcParam->sSpatialLayers[iLayerIdx];
	SSliceArgument* pSliceArgument = &pLayerCfg->sSliceArgument;

	InitParam();
	InitLayerSliceBuffer(0);
	InitFrameBsBuffer();

	//param validation
	int32_t iRet = m_pEncoder->InitializeExt((SEncParamExt*)m_EncContext.pSvcParam);
	ASSERT_TRUE(cmResultSuccess == iRet);


	//
	pCtx->pCurDqLayer = pCtx->ppDqLayerList[iLayerIdx];
	iRet = InitAllSlicesInThread(pCtx);
	ASSERT_TRUE(cmResultSuccess == iRet);

	//
	SimulateSliceInOneLayer();


	iRet = ReOrderSliceInLayer(pCtx, pSliceArgument->uiSliceMode, pCtx->iActiveThreadsNum);
	ASSERT_TRUE(cmResultSuccess == iRet);

	int32_t iCodedSlcNum = pCtx->pCurDqLayer->sSliceEncCtx.iSliceNumInFrame;
	int32_t iMaxSlicNum = pCtx->pCurDqLayer->iMaxSliceNum;
	ASSERT_TRUE(iCodedSlcNum <= iMaxSlicNum);
	ASSERT_TRUE(NULL != pCtx->pCurDqLayer);
	ASSERT_TRUE(NULL != pCtx->pCurDqLayer->ppSliceInLayer);
	for (int32_t iSlcIdx = 0; iSlcIdx < iCodedSlcNum; iSlcIdx++) {
		ASSERT_TRUE(NULL != &pCtx->pCurDqLayer->ppSliceInLayer[iSlcIdx]);
		ASSERT_TRUE(iSlcIdx == pCtx->pCurDqLayer->ppSliceInLayer[iSlcIdx]->iSliceIdx);
	}

	//
	iRet = m_pEncoder->Uninitialize();
	ASSERT_TRUE(cmResultSuccess == iRet);

	UnInitFrameBsBuffer();
	UnInitLayerSliceBuffer(0);
	UnInitParam();
}


void ParamSetForReallocateTest(sWelsEncCtx* pCtx , int32_t iLayerIdx,
	                             int32_t iThreadIndex, int32_t iPartitionNum) {
	SSpatialLayerConfig* pLayerCfg = &pCtx->pSvcParam->sSpatialLayers[iLayerIdx];
	int32_t iPartitionID = rand() % iPartitionNum;
	int32_t iMBNumInPatition = 0;
	int32_t iMaxSlcNum = pCtx->pCurDqLayer->sSliceThreadInfo[iThreadIndex].iMaxSliceNum;
	int32_t iCodedSlcNum = iMaxSlcNum - 1;
	int32_t iLastCodeSlcIdx = iPartitionID + iCodedSlcNum * iPartitionNum;
	SSlice* pLastCodedSlc = &pCtx->pCurDqLayer->sSliceThreadInfo[iThreadIndex].pSliceInThread[iCodedSlcNum - 1];
	pLastCodedSlc->iSliceIdx = iLastCodeSlcIdx;

	SetPartitonMBNum(pCtx->ppDqLayerList[iLayerIdx], pLayerCfg, iPartitionNum);

	iMBNumInPatition = pCtx->pCurDqLayer->EndMbIdxOfPartition[iPartitionID];
	pCtx->pCurDqLayer->sSliceThreadInfo[iThreadIndex].iCodedSliceNum = iCodedSlcNum;
	pCtx->pCurDqLayer->LastCodedMbIdxOfPartition[iPartitionID] = rand() % iMBNumInPatition + 1;

}

TEST_F(CSliceBufferReallocatTest, ReallocateTest) {
	int32_t iLayerIdx = 0;
	int32_t iRet = 0;
	sWelsEncCtx* pCtx = &m_EncContext;
	SSpatialLayerConfig* pLayerCfg = &pCtx->pSvcParam->sSpatialLayers[iLayerIdx];
	SSliceArgument* pSliceArgument = &pLayerCfg->sSliceArgument;

	InitParam();
	InitLayerSliceBuffer(iLayerIdx);
	if (SM_SIZELIMITED_SLICE != pSliceArgument->uiSliceMode && NULL != pCtx->ppDqLayerList[iLayerIdx]) {
			UnInitLayerSliceBuffer(iLayerIdx);
			pSliceArgument->uiSliceMode = SM_SIZELIMITED_SLICE;
			iRet = IntParamForSizeLimitSlcMode(pCtx, iLayerIdx);
			ASSERT_TRUE(ENC_RETURN_SUCCESS == iRet);
			ASSERT_TRUE(NULL != pCtx->ppDqLayerList[iLayerIdx]);
	}
	InitFrameBsBuffer();

	//param validation
	iRet = m_pEncoder->InitializeExt((SEncParamExt*)m_EncContext.pSvcParam);
	ASSERT_TRUE(cmResultSuccess == iRet);


	//
	pCtx->pCurDqLayer = pCtx->ppDqLayerList[iLayerIdx];
	iRet = InitAllSlicesInThread(pCtx);
	ASSERT_TRUE(cmResultSuccess == iRet);

  int32_t iThreadIndex     = rand() % pCtx->iActiveThreadsNum;
	int32_t iPartitionNum    = rand() % pCtx->iActiveThreadsNum + 1; // for partiontion num less than thread num case

	ParamSetForReallocateTest(pCtx, iLayerIdx, iThreadIndex, iPartitionNum);

	iRet = ReallocateSliceInThread(pCtx, pCtx->pCurDqLayer, iLayerIdx, iThreadIndex);
	ASSERT_TRUE(cmResultSuccess == iRet);
	ASSERT_TRUE(NULL != pCtx->pCurDqLayer->sSliceThreadInfo[iThreadIndex].pSliceInThread);






	//
	iRet = m_pEncoder->Uninitialize();
	ASSERT_TRUE(cmResultSuccess == iRet);

	UnInitFrameBsBuffer();
	UnInitLayerSliceBuffer(0);
	UnInitParam();


}
	/*iRet      = InitWithParam(m_pEncoder, pCtx->pSvcParam);
	ASSERT_EQ (iRet, 0);
	iCodedSliceNum = GetInitialSliceNum(pSliceArgument);
	ASSERT_LT(iCodedSliceNum, 0);

	int32_t InitSliceList (sWelsEncCtx* pCtx,
                       SDqLayer* pDqLayer,
                       SSlice*& pSliceList,
                       const int32_t kiMaxSliceNum,
                       const int32_t kiDlayerIndex,
                       CMemoryAlign* pMa) {



  //init slice idex in all thread slice buffer
	for (int32_t iSlcIdx = 0; iSlcIdx < iCodedSliceNum; iSlcIdx++) {
		iThrdIdx = rand() % pCtx->pSvcParam->iMultipleThreadIdc;
		m_pEncoder->m_pEncContext->

	}

	/*
	assert (iThreadNum > 0);
	if( SM_SIZELIMITED_SLICE == eSlicMode && iThreadNum > 1) {
	iMaxSliceNum = pDqLayer->iMaxSliceNum / iThreadNum + 1;
	} else {
	iMaxSliceNum = pDqLayer->iMaxSliceNum;
  }


	int32_t ReOrderSliceInLayer(sWelsEncCtx* pCtx,
		const SliceModeEnum kuiSliceMode,
		const int32_t kiThreadNum);

	int32_t ReallocateSliceList(sWelsEncCtx* pCtx,
		SSliceArgument* pSliceArgument,
		SSlice*& pSliceList,
		const int32_t kiMaxSliceNumOld,
		const int32_t kiMaxSliceNumNew);

	int32_t ReallocateSliceInThread(sWelsEncCtx* pCtx,
		SDqLayer* pDqLayer,
		const int32_t kiDlayerIdx,
		const int32_t kiThreadIndex);

		*/

static const EncodeFileParam kFileParamArray[] = {
	{ "res/CiscoVT2people_320x192_12fps.yuv", 320, 192, 12.0f },
	//{ "res/CiscoVT2people_320x192_12fps.yuv", 320, 192, 12.0f },
	//{ "res/CiscoVT2people_320x192_12fps.yuv", 320, 192, 12.0f },
	//{ "res/CiscoVT2people_320x192_12fps.yuv", 320, 192, 12.0f },
};