cmd_kernel/time/built-in.a := rm -f kernel/time/built-in.a; arm-linux-gnueabihf-ar cDPrST kernel/time/built-in.a kernel/time/time.o kernel/time/timer.o kernel/time/hrtimer.o kernel/time/timekeeping.o kernel/time/ntp.o kernel/time/clocksource.o kernel/time/jiffies.o kernel/time/timer_list.o kernel/time/timeconv.o kernel/time/timecounter.o kernel/time/alarmtimer.o kernel/time/posix-timers.o kernel/time/posix-cpu-timers.o kernel/time/posix-clock.o kernel/time/itimer.o kernel/time/clockevents.o kernel/time/tick-common.o kernel/time/tick-broadcast.o kernel/time/tick-broadcast-hrtimer.o kernel/time/sched_clock.o kernel/time/tick-oneshot.o kernel/time/tick-sched.o kernel/time/vsyscall.o kernel/time/timekeeping_debug.o
