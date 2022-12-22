/*
typedef struct Coroutine Coroutine;
struct Coroutine
{
	S32 line;
	U64 start_time;
	void* data;
};

#define CoroutineBegin(coro) switch (coro->line) {case 0: coro->line = 0;
#define CoroutineYield(coro) do { coro->line = __LINE__; return; case __LINE__:;} while(0)
#define CoroutineYieldUntil(coro, condition) while (!(condition)) { CoroutineYield(coro); }
#define CoroutineWait(coro, duration) do {if (coro->start_time == 0.0f) { coro->start_time = stm_now(); } CoroutineYieldUntil(coro, stm_sec(stm_now()) > stm_sec(coro->start_time) + (F64)duration); coro->start_time = 0; } while (0)
#define CoroutineEnd(coro) do { coro->line = __LINE__; } while (0); }
#define CoroutineReset(coro) do { coro->line = 0; } while (0); }
#define CoroutineIsFinished(coro) (coro->line == -1)
*/

typedef struct coroutine_t
{
    float elapsed;
    unsigned cursorPosition;
    int index;
} coroutine_t;

#define COROUTINE_START(co) do { switch (co->index) { default:
#define COROUTINE_CASE(co, name) case __LINE__: name: co->index = __LINE__;
#define COROUTINE_EXIT(co) do { goto __co_end; } while (0)
#define COROUTINE_YIELD(co) do { co->index = __LINE__; COROUTINE_EXIT(co); case __LINE__:; } while (0)
#define COROUTINE_WAIT(co, time, dt) do { case __LINE__: co->index = __LINE__; co->elapsed += dt; do { if (co->elapsed < time) { goto __co_end; } else { co->elapsed = 0; } } while (0); } while (0)
#define COROUTINE_END(co) } co->index = 0; __co_end:; } while (0)

