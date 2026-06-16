#ifndef _mykernel_h_
#define _mykernel_h_

#include "circle_kernel.h"
#include <circle/types.h>

class CMyKernel : public CKernel
{
public:
    CMyKernel(void);
    ~CMyKernel(void);

    static CMyKernel &Instance()
    {
        return *s_pInstance;
    }

    // CKernelの基本メソッドをオーバーライド
    virtual TShutdownMode Run(void) override;

    // タイマー割り込みハンドラをオーバーライド
    // (引数の型はCircleのCTimer::RegisterHandlerの仕様に準拠)
    static void TimerHandler(void);

private:
    static CMyKernel *s_pInstance;
};

#endif