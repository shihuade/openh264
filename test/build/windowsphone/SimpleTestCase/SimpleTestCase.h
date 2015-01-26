#pragma once


#if (defined(ANDROID_NDK)||defined(APPLE_IOS) || defined(_WINDOWS_PHONE_))
int CodecUtMain(int argc, char** argv);
int TestAll();
#else
int main(int argc, char** argv);

#endif
