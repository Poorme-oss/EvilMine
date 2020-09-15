if (WITH_ASM AND NOT evildragon_ARM AND CMAKE_SIZEOF_VOID_P EQUAL 8)
    set(evildragon_ASM_LIBRARY "evildragon-asm")

    if (CMAKE_C_COMPILER_ID MATCHES MSVC)
        enable_language(ASM_MASM)

        if (MSVC_TOOLSET_VERSION GREATER_EQUAL 141)
            set(evildragon_ASM_FILES
                "src/crypto/cn/asm/cn_main_loop.asm"
                "src/crypto/cn/asm/CryptonightR_template.asm"
            )
        else()
            set(evildragon_ASM_FILES
                "src/crypto/cn/asm/win64/cn_main_loop.asm"
                "src/crypto/cn/asm/win64/CryptonightR_template.asm"
            )
        endif()

        set_property(SOURCE ${evildragon_ASM_FILES} PROPERTY ASM_MASM)
    else()
        enable_language(ASM)

        if (WIN32 AND CMAKE_C_COMPILER_ID MATCHES GNU)
            set(evildragon_ASM_FILES
                "src/crypto/cn/asm/win64/cn_main_loop.S"
                "src/crypto/cn/asm/CryptonightR_template.S"
            )
        else()
            set(evildragon_ASM_FILES
                "src/crypto/cn/asm/cn_main_loop.S"
                "src/crypto/cn/asm/CryptonightR_template.S"
            )
        endif()

        set_property(SOURCE ${evildragon_ASM_FILES} PROPERTY C)
    endif()

    add_library(${evildragon_ASM_LIBRARY} STATIC ${evildragon_ASM_FILES})
    set(evildragon_ASM_SOURCES
        src/crypto/common/Assembly.h
        src/crypto/common/Assembly.cpp
        src/crypto/cn/r/CryptonightR_gen.cpp
        )
    set_property(TARGET ${evildragon_ASM_LIBRARY} PROPERTY LINKER_LANGUAGE C)

    add_definitions(/Devildragon_FEATURE_ASM)
else()
    set(evildragon_ASM_SOURCES "")
    set(evildragon_ASM_LIBRARY "")

    remove_definitions(/Devildragon_FEATURE_ASM)
endif()
