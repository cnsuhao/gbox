-- project
set_project("gbox")

-- version
set_version("1.0.4")

-- set warning all as error
set_warnings("all", "error")

-- set language: c99, c++11
set_languages("c99", "cxx11")

-- disable some compiler errors
add_cxflags("-Wno-error=deprecated-declarations")
add_mxflags("-Wno-error=deprecated-declarations")

-- the debug mode
if is_mode("debug") then
    
    -- enable the debug symbols
    set_symbols("debug")

    -- disable optimization
    set_optimize("none")

    -- add defines for debug
    add_defines("__tb_debug__")

    -- attempt to enable some checkers for pc
    if is_mode("check") and is_arch("i386", "x86_64") then
        add_cxflags("-fsanitize=address", "-ftrapv")
        add_mxflags("-fsanitize=address", "-ftrapv")
        add_ldflags("-fsanitize=address")
    end
end

-- the release or profile is_mode
if is_mode("release", "profile") then

    -- the release mode
    if is_mode("release") then
        
        -- set the symbols visibility: hidden
        set_symbols("hidden")

        -- strip all symbols
        set_strip("all")

        -- fomit the frame pointer
        add_cxflags("-fomit-frame-pointer")
        add_mxflags("-fomit-frame-pointer")

    -- the profile mode
    else
    
        -- enable the debug symbols
        set_symbols("debug")

    end

    -- smallest?
    if is_option("smallest") then
 
        -- enable smallest optimization
        set_optimize("smallest")
    else
        -- enable fastest optimization
        set_optimize("fastest")
    end

    -- attempt to add vector extensions 
    add_vectorexts("sse2", "sse3", "ssse3", "mmx")
end

-- smallest?
if is_option("smallest") then

    -- add defines for small
    add_defines("__tb_small__")

    -- add defines to config.h
    add_defines_h("$(prefix)_SMALL")
end

-- for the windows platform (msvc)
if is_plat("windows") then 

    -- the release mode
    if is_mode("release") then

        -- link libcmt.lib
        add_cxflags("-MT") 

    -- the debug mode
    elseif is_mode("debug") then

        -- enable some checkers
        add_cxflags("-Gs", "-RTC1") 

        -- link libcmtd.lib
        add_cxflags("-MTd") 
    end

    -- no msvcrt.lib
    add_ldflags("-nodefaultlib:\"msvcrt.lib\"")
end

-- add option: demo
option("demo")
    set_enable(true)
    set_showmenu(true)
    set_category("option")
    set_description("Enable or disable the demo module")

-- add packages
add_packagedirs("pkg") 

-- add projects
add_subdirs("src/gbox") 
if is_option("demo") then add_subdirs("src/demo") end
