### Ⅰ. 의문점 ( 판도라 )
#### 1️⃣ Socket Thread Close
✅ 스레드 종료

(1). pthread_join(pthread_t pth, void** status) 하기 전까지, 해당 스레드가 종료되었는지 알 수 있는 방법이 없다. <br/>
(2). 그럼 스레드가 **정상적으로 종료되었는지** 확인을 어떻게 하는가? <br/>
&nbsp; &nbsp; &nbsp; &nbsp; ▪ pool_destroy : 중복하여, 해당 자원을 호출 또는 제거를 방지하기 위함 <br/>
(3). pthread_t* pth 로 동적 할당한 스레드의 경우, 언제 **memfree()** 를 해주어야 하는가

✅ 코드 분석
- Disconnect
  - (1). Session->SocketClose()
    - CS_TcpSocket_Stop()
      - pTcpSock->bStop = true 으로 Run 돌고 있었던, 스레드 종료
  - (2). Session->Close()
    - // 이거는 또 무슨 기능

- 완전히 해제 여부
  - (1). bNeedToFree : Free 를 해야한다 의 개념 ( 중요하지 X ) ( default : true )
  - (2). bGCDelete : ( default : true )
    - __tcpsocket_GarbageCollector_Add(pTcpSock); ( **현재 사용하는 코드** )
      - // __tcpsocket_GarbageCollector_Run((MAX_GC_TIMEOUT/2));
        - MAX_GC_TIMEOUT 60초 --> 30초가 지나면, memFree 하도록 되어있다.
      - tcpClose() : tcpClose 이후 **tcpSocketGC** 에 들어가게 된다.
    - ( default : **no** ) 일 경우, 이런식으로 바로 자원을 Free 하게 되는데, 이는 만약에 스레드가 종료 중에 있거나, 종료 예약만 할 뿐 아직 종료되지 않은 시점이라면, 문제가 발생하는 경우가 아닌가 
      - __tcpsocket_Resource_Free(pTcpSock);
        - tcp 안에 내부의 자원 해제 ( ex. event, QOS
			- CS_OAL_Free(pTcpSock);
        - CS_MemFree_impl // 그냥 free 하게 된다.
       
      ** <b>local_thread_Destroy</b>
          && struct cs_task* task = (struct cs_task*)taskId; 이 과정이
          && 변수를 보면,
            struct cs_task
{
	pthread_t task_;
	struct cs_task_arg targ_;
	TAILQ_ENTRY(cs_task) entries;
}; 이렇게 되어 있어서 저 task->task_ 를 참조할 수 있게 되어 있음
        - rc = pthread_cancel(task->task_); // (1). 종료 예약
        - pthread_join 을 통해서 해당 pthread_t task_ 가 진짜 종료되었는지까지 확인한다. (2). join 까지 해서 확실하게 확인
        - CS_OAL_Free(task); // (3). 완전히 메모리 해제
  - (3). bIsDeleted
    -- __tcpsocket_GarbageCollector_Add() 여기  GC Queue 에 넣는 순간 이미,
      * hTCP->bIsDeleted = true;
	hTCP->closeTime = GET_CURRENT_SECOND;
	s_TCPSockGCQueue.push(hTCP); // 이렇게 3개 설정
- 저 m_task_id 는 언제 사라지는 것인지
- CS_Thread_JoinTimeout --> 저게 의미하는 바가 무엇인지
  - 일정 시간 이내에 Join 하라는 것인지
  - 아니면, join 을 저 timeout 이내에 못하면 강제종료 하라는 것안자
 
rc = pthread_setcancelstate(PTHREAD_CANCEL_ENABLE, NULL);
   posix 스레드에서 사용되며, 스레드의 취소 상태를 설정하는 함수이다.
   스레드의 취소 상태는
     (1)., ENABLED
         다른 스레드에서 pthread_cancel 함수를 호출하면 취소되고, 그에 따라 취소 지점에서 즉시 종료될 수 있다 ( **너무 당연한 말 아닌가** )
      (2). DISABLED 로 설정
        -- 다른 스레드에서 pthread_cancle 함수 호출에 의해 바로 종료되지 않는다 (?)
            --> 그러면 ? // 취소 지점에서는 즉시 종료되지만, 임계적인 영역에서 작업중이거나 중요한 작업을 수행하는 동안 취소를 방지하기 위함
