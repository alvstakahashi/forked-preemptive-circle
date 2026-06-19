//
// main.c
//
// Circle - A C++ bare metal environment for Raspberry Pi
// Copyright (C) 2014  R. Stange <rsta2@o2online.de>
//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.
//

#include <circle/startup.h>

#include <stdio.h>
#include <stdint.h>

#include <kernel.h>
#include "kernel_cfg.h"
#include "kernel_impl.h"
#include "target_kernel.h"

#include "circle_kernel.h"
#include "mykernel.h"
#include "task-main.h"

static const char FromKernel2[] = "kernel";
static const char FromTask[] = "Task";
volatile int count = 0;

int main(void)
{
	// 作成するクラスを CMyKernel に変更
	CMyKernel Kernel; // ⭕ mainの中なので安全に初期化される

	// 実行（ここから CMyKernel::Run() が呼び出される）
	TShutdownMode ShutdownMode = Kernel.Run();

	switch (ShutdownMode)
	{
	case ShutdownReboot:
		reboot();
		return EXIT_REBOOT;

	case ShutdownHalt:
	default:
		halt();
		return EXIT_HALT;
	}
}
#if NO_SSP

uint32_t tick_count;
void _kernel_handler(INTHDR userhandler)
{
	tick_count++;
	if (tick_count >= 100U)
	{
		CMyKernel::Instance().m_Logger.Write(FromKernel2, LogNotice, "1Second Up.");
		tick_count = 0;
	}
}
void sta_ker(void)
{
	tick_count = 0;
	task2(0);
	task3(0);
}
#endif

void main_task(intptr_t arg)
{
	CMyKernel::Instance().m_Logger.Write(FromKernel2, LogNotice, "main here");
	act_tsk(LOW_TASK3_ID);
	act_tsk(HIGH_TASK2_ID);
	CMyKernel::Instance().m_Logger.Write(FromKernel2, LogNotice, "main end");
}

void High_task(intptr_t arg)
{
	int toggle= 0;
	CMyKernel::Instance().m_Logger.Write(FromTask, LogNotice, "task2 RUNNING-----------------------------------------------------");
	for(;;)
	{
		if ((toggle ^= 1) != 0)
		{
			CMyKernel::Instance().m_Logger.Write(FromTask, LogNotice, "TASK2 LED ON  count= %d",count);
			CMyKernel::Instance().m_ActLED.On();
		}
		else
		{
			CMyKernel::Instance().m_Logger.Write(FromTask, LogNotice, "TASK2 LED OFF count= %d",count);
			CMyKernel::Instance().m_ActLED.Off();
		}
		dly_tsk(1000);
	}
}

/*
Low_tsk is a task with low task priority.
Low_tsk will try to run continuously after it starts.
The circle library's MsDelay keeps the task in the RUN state due to a busy loop.
However, if a high-priority High_task is in the Ready state, i.e., tries to run,
Low_task will immediately be preempted and switched to High_task.
This will probably happen during Low_task's 5-second delay, but the delay time should be exactly 5 seconds.
*/

void Low_task(intptr_t arg)
{
	CMyKernel::Instance().m_Logger.Write(FromTask, LogNotice, "task3-----------------");
	for(;;)
	{
		CTimer::Get ()->MsDelay (5000);		// busy Delay 5 Secons
		count++;
		CMyKernel::Instance().m_Logger.Write(FromTask, LogNotice, "task3-----------------");
	}
}