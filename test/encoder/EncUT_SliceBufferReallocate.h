#include <gtest/gtest.h>
#include "utils/FileInputStream.h"
#include "svc_encode_slice.h"
#include "WelsEncoderExt.h"

#define MAX_WIDTH  (4096)
#define MAX_HEIGH  (2304)

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

			iRet = m_pEncoder->GetDefaultParams(m_EncContext.pSvcParam);
			ASSERT_EQ(0, iRet);

			int32_t iCacheLineSize = 16;
			m_EncContext.pMemAlign = new CMemoryAlign(iCacheLineSize);
			ASSERT_TRUE(NULL != m_EncContext.pMemAlign);
		}

		virtual void TearDown() {
	    WelsDestroySVCEncoder(m_pEncoder);
			m_pEncoder = NULL;

			if (m_EncContext.pMemAlign != NULL)  {
				delete m_EncContext.pMemAlign;
				m_EncContext.pMemAlign = NULL;
			}
	}

	void EncodeFile(const char* fileName, SEncParamExt* pEncParamExt);
	void EncodeStream(InputStream* in, SEncParamExt* pEncParamExt);
	void InitParam();
	void InitFrameBsBuffer();
	void UnInitFrameBsBuffer();
	void InitLayerSliceBuffer(const int32_t iLayerIdx);
	void UnInitLayerSliceBuffer(const int32_t iLayerIdx);

	ISVCEncoder*  m_pEncoder;
	sWelsEncCtx   m_EncContext;
  private:

};
