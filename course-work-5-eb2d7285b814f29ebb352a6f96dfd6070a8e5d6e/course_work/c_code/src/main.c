int main(void)
{
    volatile unsigned int* ptr = (void*)(0xA0000);
    for (unsigned long i = 0; i < 0xFFFF; ++i)
    {
        ++ptr[i];
    }
    return 0;
}

__attribute__((section(".entry")))
__attribute__((noreturn))
void _entry(void) // NOLINT Uses custom linkage, so it does not matter if it is reserved
{
    main();
    for (;;);
}
