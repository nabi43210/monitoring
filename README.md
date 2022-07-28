# monitoring

## Windows Monitoring
**스냅샷으로 windows 프로세스 목록 가져오기 --> winproc.c **
//디버그 권한 추가 완료

**NT Legacy 드라이버로 EPROESS구조체 접근하여 프로세스 정보 가져오기 --> PS_DRIVER_1**
- 결과 출력은 함수 내 DbgPrint()를 사용했음: DbgView.exe로 화면으로 확인 가능함
- 드라이버 빌드하는법
    * Enterprise WDK 사용
    * msbuild 명령어 사용하여 빌드 : msbuild 경로\PS_DRIVER_1.vcxproj /p:platform=x64
    * 주의: 프로젝트 속성 -> 경고 수준 W3으로 낮추고 '경고를 오류로 처리'=아니요(/Wx-)
- 드라이버 서비스 만들기
    * cmd관리자권한 실행
    * sc create 드라이버명 binpath="경로\PD_DRIV.sys" displayname="드라이버명" start=demand type=kernel
- 드라이버 메모리에 올리기(실행)
    * 운영체제를 디버그모드로 변경
        * bcdedit.exe -set TESTSIGING ON
        * 재부팅
    * DbgView.exe 실행 (print출력용)
    * cmd관리자권한 실행
    * sc start 드라이버명
- 64비트 환경에서 제작하였으므로 Win32환경에서 작동하지 않음
    - 드라이버 실행하기 위해서는 인증서를 얻어와야하나 운영체제를 테스트모드로 재부팅하는 것으로 실행까지는 가능
- 드라이버 코드 return을 UNSUCCESSFUL 로 설정하여 드라이버 실행과 동시에 메모리에서 자동으로 내려감
    -> 따로 메모리공간에서 드라이버를 해제할 필요 없음    
