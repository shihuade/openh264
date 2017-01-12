#include <gtest/gtest.h>
#include "codec_def.h"
#include "codec_app_def.h"
#include "codec_api.h"
#include "utils/FileInputStream.h"
#include "svc_encode_slice.h"

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
