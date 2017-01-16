#include "wels_common_basis.h"
#include "utils/BufferedData.h"
#include "BaseEncoderTest.h"
#include "svc_encode_slice.h"
#include "encoder.h"
#include "EncUT_SliceBufferReallocate.h"


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

TEST_F(CSliceBufferReallocatTest, ReallocateTest) {
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
}

TEST_F(CSliceBufferReallocatTest, ReOrderSliceInLayer) {

	const int32_t iLayerIdx = 0;
	int32_t iRet            = 0;
	int32_t iCodedSliceNum  = 0;
	int32_t iThrdIdx        = 0;
	float fCompressRatioThr = COMPRESS_RATIO_THR;
	SSpatialLayerConfig* pLayerCfg = &pCtx->pSvcParam->sSpatialLayers[iLayerIdx];
	SSliceArgument* pSliceArgument = &pLayerCfg->sSliceArgument;
	sWelsEncCtx* pCtx = &m_EncContext;

	pCtx->pSvcParam->iMultipleThreadIdc = (rand() % MAX_THREADS_NUM) + 1;
	pSliceArgument->uiSliceMode                = (SliceModeEnum) (rand() % 4);
	pCtx->pSvcParam->iSpatialLayerNum   = 1;
	pLayerCfg->iVideoWidth                     = WelsClip3(((rand() + 15) >> 4 + 1) << 4, 16, MAX_WIDTH);
	pLayerCfg->iVideoHeight                    = WelsClip3(((rand() + 15) >> 4 + 1) << 4, 16, MAX_HEIGH);

		//InitSliceList(m_EncContext,);
	

	const int32_t kiSpsSize          = 1 * SPS_BUFFER_SIZE;
	const int32_t kiPpsSize          = 1 * PPS_BUFFER_SIZE;
	int32_t iNonVclLayersBsSizeCount = SSEI_BUFFER_SIZE + kiSpsSize + kiPpsSize;
	int32_t iLayerBsSize             = WELS_ROUND (((3 * pLayerCfg->iVideoWidth * pLayerCfg->iVideoHeight) >> 1) * fCompressRatioThr) + MAX_MACROBLOCK_SIZE_IN_BYTE_x2;
	int32_t iVclLayersBsSizeCount    = WELS_ALIGN(iLayerBsSize, 4);
	int32_t iCountBsLen              = iNonVclLayersBsSizeCount + iVclLayersBsSizeCount;
	int32_t iCountNals               = 10;


	  // Output
  pCtx->pOut = (SWelsEncoderOutput*)m_MemoryAlign.WelsMallocz (sizeof (SWelsEncoderOutput), "SWelsEncoderOutput");
  WELS_VERIFY_RETURN_PROC_IF (1, (NULL == pCtx->pOut), FreeMemorySvc (&pCtx))
  pCtx->pOut->pBsBuffer = (uint8_t*)m_MemoryAlign.WelsMallocz (iCountBsLen, "pOut->pBsBuffer");
  WELS_VERIFY_RETURN_PROC_IF (1, (NULL == pCtx->pOut->pBsBuffer), FreeMemorySvc (m_EncContext))
  pCtx->pOut->uiSize = iCountBsLen;
  pCtx->pOut->sNalList = (SWelsNalRaw*)m_MemoryAlign.WelsMallocz (iCountNals * sizeof (SWelsNalRaw), "pOut->sNalList");
  WELS_VERIFY_RETURN_PROC_IF (1, (NULL == pCtx->pOut->sNalList), FreeMemorySvc (m_EncContext))
  pCtx->pOut->pNalLen = (int32_t*)m_MemoryAlign.WelsMallocz (iCountNals * sizeof (int32_t), "pOut->pNalLen");
  WELS_VERIFY_RETURN_PROC_IF (1, (NULL == pCtx->pOut->pNalLen), FreeMemorySvc (ppCtx))
  pCtx->pOut->iCountNals    = iCountNals;
  pCtx->pOut->iNalIndex     = 0;
  pCtx->pOut->iLayerBsIndex = 0;


  pCtx->pFrameBs = (uint8_t*)m_MemoryAlign.WelsMalloc (iTotalLength, "pFrameBs");
  WELS_VERIFY_RETURN_PROC_IF (1, (NULL == pCtx->pFrameBs), FreeMemorySvc (ppCtx))
  pCtx->iFrameBsSize = iTotalLength;
  pCtx->iPosBsBuffer = 0;

	

	/*iRet      = InitWithParam(m_pEncoder, pCtx->pSvcParam);
	ASSERT_EQ (iRet, 0);
	iCodedSliceNum = GetInitialSliceNum(pSliceArgument);
	ASSERT_LT(iCodedSliceNum, 0);
	*/

	/*
	int32_t InitSliceList (sWelsEncCtx* pCtx,
                       SDqLayer* pDqLayer,
                       SSlice*& pSliceList,
                       const int32_t kiMaxSliceNum,
                       const int32_t kiDlayerIndex,
                       CMemoryAlign* pMa) {

	*/

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

}

static const EncodeFileParam kFileParamArray[] = {
	{ "res/CiscoVT2people_320x192_12fps.yuv", 320, 192, 12.0f },
	//{ "res/CiscoVT2people_320x192_12fps.yuv", 320, 192, 12.0f },
	//{ "res/CiscoVT2people_320x192_12fps.yuv", 320, 192, 12.0f },
	//{ "res/CiscoVT2people_320x192_12fps.yuv", 320, 192, 12.0f },
};