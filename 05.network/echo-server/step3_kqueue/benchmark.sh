#!/bin/bash

# 웹서버가 실행 중인지 확인
if ! nc -z localhost 8080; then
    echo "HTTP 서버가 실행 중이지 않습니다. 먼저 서버를 시작하세요."
    exit 1
fi

echo "=== kqueue 기반 HTTP 서버 성능 테스트 ==="
echo ""

# 테스트 케이스 정의
CONCURRENCY_LEVELS=(1 10 50 100)
REQUESTS=1000  # 더 적은 요청 수로 테스트

# 테스트 시작
for c in "${CONCURRENCY_LEVELS[@]}"; do
    echo "동시 연결 $c 개로 테스트 중..."
    
    # 결과 파일에 저장 (keepalive 옵션 제거)
    ab -n $REQUESTS -c $c http://localhost:8080/ > ab_result.txt 2>&1
    
    # 파일에서 결과 확인 (BSD grep용으로 수정)
    req_per_sec=$(grep "Requests per second" ab_result.txt | awk '{print $4}')
    transfer_rate=$(grep "Transfer rate" ab_result.txt | awk '{print $3}')
    mean_time=$(grep "Time per request" ab_result.txt | head -1 | awk '{print $4}')
    
    echo "  요청/초: ${req_per_sec:-데이터 없음}"
    echo "  전송률: ${transfer_rate:-데이터 없음} KB/s"
    echo "  평균 응답 시간: ${mean_time:-데이터 없음} ms"
    
    # ab 출력 확인을 위해 임시로 사용 가능
    echo "  [ab 상태: $(grep "Complete requests" ab_result.txt | awk '{print $3}' || echo "데이터 없음") 완료]"
    echo ""
    
    # 다음 테스트 전 잠시 대기
    sleep 2
done

# 임시 파일 삭제
rm -f ab_result.txt

echo "=== 테스트 완료 ==="
