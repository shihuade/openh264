#include <gtest/gtest.h>
#include "codec_def.h"
#include "codec_app_def.h"
#include "codec_api.h"
#include "wels_common_basis.h"
#include "utils/BufferedData.h"
#include "utils/FileInputStream.h"
#include "BaseEncoderTest.h"


struct EncodeFileParam {
	const char* pkcFileName;
	int iWidth;
	int iHeight;
	float fFrameRate;
};

class CSliceBufferReallocatTest : public ::testing::Test { //WithParamInterface<EncodeFileParam>{
  public:
		virtual void SetUp() {
			m_pEncoder   = NULL;
	    int32_t iRet = WelsCreateSVCEncoder(&m_pEncoder);
	    ASSERT_EQ(0, iRet);
	    ASSERT_TRUE(m_pEncoder != NULL);

			iRet = m_pEncoder->GetDefaultParams(&m_EncParamExt);
			ASSERT_EQ(0, iRet);
		}

		virtual void TearDown() {
	    WelsDestroySVCEncoder(m_pEncoder);
			m_pEncoder = NULL;
	}

	void EncodeFile(const char* fileName, SEncParamExt* pEncParamExt);
	void EncodeStream(InputStream* in, SEncParamExt* pEncParamExt);

	ISVCEncoder*  m_pEncoder;
	SEncParamExt  m_EncParamExt;

  private:

};

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

TEST_F(CSliceBufferReallocatTest, ReallocateTest02) {
	EncodeFileParam pEncFileParam; // = GetParam();
	pEncFileParam.pkcFileName = "res/CiscoVT2people_320x192_12fps.yuv";
	pEncFileParam.iWidth = 320;
	pEncFileParam.iHeight = 192;
	pEncFileParam.fFrameRate = 12.0;
	FileInputStream fileStream;
	ASSERT_TRUE(fileStream.Open(pEncFileParam.pkcFileName));

	m_EncParamExt.iPicHeight = pEncFileParam.iHeight;
	m_EncParamExt.iPicWidth = pEncFileParam.iWidth;
	m_EncParamExt.fMaxFrameRate = pEncFileParam.fFrameRate;
	EncodeStream(&fileStream, &m_EncParamExt);
}

static const EncodeFileParam kFileParamArray[] = {
	{ "res/CiscoVT2people_320x192_12fps.yuv", 320, 192, 12.0f },
	//{ "res/CiscoVT2people_320x192_12fps.yuv", 320, 192, 12.0f },
	//{ "res/CiscoVT2people_320x192_12fps.yuv", 320, 192, 12.0f },
	//{ "res/CiscoVT2people_320x192_12fps.yuv", 320, 192, 12.0f },
};