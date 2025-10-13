
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <string.h>
#include <libcc.h>

// 信号处理函数
#ifndef __CC_WINDOWS__
static void sig_handler(int sig_num, siginfo_t *info, void *ucontext) {
#else
static void sig_handler(int sig_num) {
#endif

    const char* sig_name = "";
    switch(sig_num) {
        case SIGINT:  sig_name = "SIGINT(Ctrl+C)"; break;
        case SIGILL:  sig_name = "SIGILL(非法指令)"; break;
        case SIGFPE:  sig_name = "SIGFPE(浮点异常)"; break;
        case SIGSEGV: sig_name = "SIGSEGV(段错误)"; break;
        case SIGTERM: sig_name = "SIGTERM(终止请求)"; break;
    #ifdef __CC_WINDOWS__
        case SIGBREAK:sig_name = "SIGBREAK(Ctrl+Break)"; break;
    #endif
        case SIGABRT: sig_name = "SIGABRT(abort调用)"; break;
        default:      sig_name = "未知信号";
    }
    printf("\n捕获信号: %d(%s)\n", sig_num, sig_name);
    
    // 特定信号处理逻辑Ctrl + C
    if (sig_num == SIGINT) {
        printf("程序终止\n");
    } else {
        tchar_t strings[1024 * 4];
        size_t size = 0;
        fprintf(stderr, "Program received signal %s.\n", strsignal(sig_num));
        fprintf(stderr, "%d:%s\n", sig_num, strerror(sig_num));
        size = _cc_get_resolve_symbol(strings, _cc_countof(strings));
        if (size > 0) {
            fprintf(stderr, "#%.*s\n", (int)size, strings);
        }
    }

    _exit(EXIT_SUCCESS);
}

static void sig(void) {
    int i;
    int signals[] = {
        SIGINT,  /* Ctrl + C */
        SIGILL,  /* Illegal instruction (ANSI).  */
        SIGABRT, /* Abort (ANSI).  */
        SIGBUS,  /* BUS error (4.2 BSD). (unaligned access) */
        SIGFPE,  /* Floating-point exception (ANSI).  */
        SIGSEGV, /* Segmentation violation (ANSI).  */
        SIGTERM, /* Termination (ANSI).  */
#ifdef __CC_WINDOWS__
        SIGBREAK,/* Ctrl + Break */
#endif
    };
#ifdef __CC_WINDOWS__
    for (i = 0; i < _cc_countof(signals); ++i) {
        signal(signals[i], sig_handler);
    }
#else
    struct sigaction sa;
    memset(&sa, 0, sizeof(struct sigaction));
    sa.sa_sigaction = sig_handler;
    sa.sa_flags = SA_RESTART | SA_SIGINFO;

    for (i = 0; i < _cc_countof(signals); ++i) {
        if (sigaction(signals[i], &sa, NULL) != 0) {
            fprintf(stderr, "failed to set signal handler for %s(%d)!\n", strsignal(signals[i]), signals[i]);
            break;
        }
    }
#endif
}

// 触发SIGFPE的测试函数
void trigger_fpe() {
    int a = 1, b = 0;
    printf("%d\n", a/b); // 除零错误
}

int main() {
    // 注册信号处理函数
    sig();

    printf("可用操作:\n");
    printf("1. Ctrl+C 发送SIGINT\n");
    printf("2. 输入'test'触发SIGFPE(浮点异常)\n");
    printf("3. 输入'segv'触发SIGSEGV(段错误)\n");
    printf("4. 输入'abort'触发SIGABRT\n");
    printf("5. 其他终端执行: kill -TERM\n");

    char cmd[32];
    while(1) {
        printf("\n> ");
        scanf("%s", cmd);
        
        if (strcmp(cmd, "test") == 0) {
            trigger_fpe();
        } 
        else if (strcmp(cmd, "segv") == 0) {
            int* p = NULL;
            *p = 42; // 空指针解引用
        }
        else if (strcmp(cmd, "abort") == 0) {
            abort();
        }
        else {
            printf("未知命令");
        }
    }
    return 0;
}
