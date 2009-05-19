/*
 This test has not every core request a global from the pipeline.
 This is to test if a remote global request is properly forwarded.
*/
    .file "sparse_globals.s"
    .text
    
    .globl main
    .align 64
main:
    mov 42, %1
    
    allocate %2, 0, 0, 0, 0
    setlimit %2, 4
    cred foo, %2
    
    ! Sync
    mov %2, %0
    end

    .align 64
foo:
    .registers 1 0 2 0 0 0
    cmp %l0, 2
    bne 1f
    print %g0, 0
1:  nop
    end
    
