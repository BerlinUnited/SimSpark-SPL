# useful for cross-compilation from multiple root directories
set(CMAKE_FIND_ROOT_PATH ${CMAKE_FIND_ROOT_PATH}
    "$ENV{CMAKE_ALTERNATIVE_ROOT}")

if (WIN32)
  set(ENV{SDLDIR} $ENV{SDLDIR} C:/library/SDL/ 
         "C:/Program Files/SDL/"
         "C:/Program Files (x86)/SDL/"
         C:/SDL/
         )
  set(CMAKE_PROGRAM_PATH ${CMAKE_PROGRAM_PATH} 
         C:/Library/
         C:/Library/ruby/bin
         )
  set(CMAKE_INCLUDE_PATH ${CMAKE_INCLUDE_PATH} 
         C:/Library/include
         C:/Library/boost
         C:/library/devil/include
         "C:/Program Files/GnuWin32/include"
         "C:/Program Files (x86)/GnuWin32/include"
         C:/library/GnuWin32/include
         C:/GnuWin32/include
         )
  set(CMAKE_LIBRARY_PATH ${CMAKE_LIBRARY_PATH}
         C:/Library/lib
         C:/Library/boost
         C:/library/devil
         "C:/Program Files/GnuWin32" 
         "C:/Program Files (x86)/GnuWin32" 
         C:/library/GnuWin32/
         C:/GnuWin32/
         )
endif (WIN32)
