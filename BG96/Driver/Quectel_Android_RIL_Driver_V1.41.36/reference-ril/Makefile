export USE_NDK=1
NDK_BUILD=/android/android-ndk/android-ndk-r10e/ndk-build
NDK_PROJECT_PATH=$(shell pwd)
NDK_DEBUG=0
APP_ABI=armeabi,x86,arm64-v8a#,mips,armeabi-v7a,mips64,x86_64
APP_PLATFORM=android-14 #8~2.2 9~2.3 14~4.0 17~4.2 19~4.4 21~5.0 22~5.1 23~6.0 24~6.x
APP_BUILD_SCRIPT=Android.mk
NDK_OUT=../out/reference-ril
NDK_LIBS_OUT=../libquectel-ril

tgz=$(shell cat reference-ril.c | grep Quectel_Android_RIL_ | cut -d '"' -f 2)

ifneq ($(wildcard ../.git/HEAD),)
tgz=$(shell cat reference-ril.c | grep Quectel_Android_RIL_ | cut -d '"' -f 2)_$(shell git status | awk '{if (NR==1) print $$3}')
$(shell echo "const char s_ql_buildinfo[] = { \\" > ql-buildinfo.c)
$(shell echo \"libquectel-ril build info: \\ >> ql-buildinfo.c)
$(shell echo `git status | awk '{if (NR==1) print $$0}'` \\ >> ql-buildinfo.c)
$(shell echo `git status | awk '{if (NR==2) print $$0}'` \\ >> ql-buildinfo.c)
$(shell echo `git rev-parse HEAD` \\ >> ql-buildinfo.c)
$(shell echo `git var -l | grep user.email` \\>> ql-buildinfo.c)
$(shell echo `date +%D_%T` \">> ql-buildinfo.c)
$(shell echo \}\; >> ql-buildinfo.c)
endif

libquectel-ril: clean
	@echo $(tgz)
	$(NDK_BUILD) V=0 NDK_OUT=$(NDK_OUT)  NDK_LIBS_OUT=$(NDK_LIBS_OUT) APP_BUILD_SCRIPT=$(APP_BUILD_SCRIPT) NDK_PROJECT_PATH=$(NDK_PROJECT_PATH) NDK_DEBUG=$(NDK_DEBUG) APP_ABI=$(APP_ABI) APP_PLATFORM=$(APP_PLATFORM)
	rm -rf ql-buildinfo.c
	cp ql-ril.conf $(NDK_LIBS_OUT)
	tar -czf ../SOURCE_$(tgz).tgz $(NDK_LIBS_OUT) ../reference-ril ../include
	tar -czf ../$(tgz).tgz $(NDK_LIBS_OUT)

clean:
	rm -rf $(NDK_OUT) $(NDK_LIBS_OUT) ../out
	find . -name "*~" | xargs rm -f
