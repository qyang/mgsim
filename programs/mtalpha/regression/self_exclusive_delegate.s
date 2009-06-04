/*
 This test test exclusive delegation to the same core the delegation comes from.
 This should work properly because the delegated create should not register as an
 exclusive create on the source side.
 */
    .file "self_exclusive_delegate.s"
    
    .globl main
    .ent main
main:
    allocate $2, 0, 0, 0, 0
    setplace $2, 5  # Delegate to core #0, exclusive
    cred $2, foo
    
    # Sync
    mov $2, $31
    end
    .end main

    .ent foo
foo:
    .registers 0 0 1 0 0 0
    print $l0, 0
    end
    .end foo

    .ascii "PLACES: 1\0"
