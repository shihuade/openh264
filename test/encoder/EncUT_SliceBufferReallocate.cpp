#include "wels_common_basis.h"
#include "utils/BufferedData.h"
#include "BaseEncoderTest.h"
#include "svc_encode_slice.h"
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
	m_EncParamExt.iUsageType    = CAMERA_VIDEO_REAL_TIME;
	m_EncParamExt.iPicHeight    = pEncFileParam.iHeight;
	m_EncParamExt.iPicWidth     = pEncFileParam.iWidth;
	m_EncParamExt.fMaxFrameRate = pEncFileParam.fFrameRate;
	EncodeStream(&fileStream, &m_EncParamExt);
}

TEST_F(CSliceBufferReallocatTest, ReOrderSliceInLayer) {

	m_EncParamExt.iMultipleThreadIdc = rand() % ( MAX_THREADS_NUM +1);
	m_EncParamExt.sSpatialLayers[0].sSliceArgument.uiSliceMode = (SliceModeEnum) (rand() % 4);
	m_EncParamExt.iSpatialLayerNum = 1;

	int iRet = InitWithParam(m_pEncoder, pEncParamExt);
	int32_t GetInitialSliceNum(SSliceArgument* pSliceArgument)
	/*
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