#include "mykernel.h"
#include "task-main.h"

static const char FromKernel3[] = "Timer";

#ifdef __cplusplus
extern "C"
{
#endif

extern void sta_ker(void);

#ifdef __cplusplus
}
#endif

uint32_t tick_count;

CMyKernel *CMyKernel::s_pInstance = nullptr;

CMyKernel::CMyKernel(void)
    : CKernel() // 親クラスであるCKernelのコンストラクタを確実に呼ぶ
{
    s_pInstance = this; // 💡 生まれた瞬間に自分の居場所を記録
}

CMyKernel::~CMyKernel(void)
{
    s_pInstance = nullptr;
}

TShutdownMode CMyKernel::Run(void)
{
    // 1. Circle本来の初期化処理（画面、シリアル、メモリ等のセットアップ）を実行
    //    内部で Initialize() が呼ばれ、デバイスの準備が完了します
    if (!Initialize())
    {
        return ShutdownHalt;
    }

    tick_count = 0;

    // 3. 定期タイマーの登録（CircleのCTimerクラス(m_Timer)を利用）
    //    引数は10ミリ秒単位, circleの1tickのデフォルトが10msなので一旦それにする
    m_Timer.RegisterPeriodicHandler(TimerHandler);

    // 4. 割り込みを許可して自作カーネルの実行フェーズへ移行
    sta_ker();
    while (1)
        ;

    // スケジューラが回り続ける限り、通常ここには戻りません
    return ShutdownHalt;
}

void CMyKernel::TimerHandler(void)
{
#if 0
    CMyKernel *pThis = &CMyKernel::Instance();

    // ⭕ ユーザー様のご指摘通り、絶対にヌルチェックを行う
    if (pThis != nullptr)
    {
        pThis->m_Logger.Write(FromKernel, LogNotice, "TimerHandler C++");
    }
#endif
    // ※ この時点で、IRQStubによってコンテキスト（レジスタ一式）はスタックに保存済みです。
    //_kernel_handler(isig_tim);
    _kernel_handler(user_handler);
}

void user_handler(void)
{
	tick_count++;
	if (tick_count >= 100U)
	{
		CMyKernel::Instance().m_Logger.Write(FromKernel3, LogNotice, "1Second Up.");
		tick_count = 0;
	}
    isig_tim();
}
