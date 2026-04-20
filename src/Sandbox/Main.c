#include <TK/TK.h>

int main()
{
    TK_AllocatorTracker tracker = TK_AllocatorTracker( &TK_AllocatorLibc );
    TK_Allocator *allocator = &tracker.allocator;
}