if (WIN32)
  macro ( folder_list result curdir )
    file(GLOB children RELATIVE ${curdir} ${curdir}/*)
    set(dirlist "")
    foreach ( child ${children})
      if(IS_DIRECTORY ${curdir}/${child})
          list(APPEND dirlist ${child})
      endif()
    endforeach()
    set(${result} ${dirlist})
    list(SORT ${result})
    list(REVERSE ${result})
  endmacro()

  folder_list(win10_ver_list "C:/Program Files (x86)/Windows Kits/10/Include")

  unset(DX12_HEADER_SEARCH_DIRS CACHE)
  unset(DX12_LIB_SEARCH_DIRS CACHE)

  foreach(win10_ver ${win10_ver_list})
    list(APPEND DX12_HEADER_SEARCH_DIRS "C:/Program Files (x86)/Windows Kits/10/Include/${win10_ver}/um")
    list(APPEND DX12_LIB_SEARCH_DIRS "C:/Program Files (x86)/Windows Kits/10/Lib/${win10_ver}/um/x64")
  endforeach()
  
  find_path(DX12_INCLUDE_DIR
    NAMES
      D3Dcompiler.h D3D12.h
    PATH_SUFFIXES Include
    PATHS
      ${DX12_HEADER_SEARCH_DIRS}
      "C:/Program Files (x86)/Microsoft DirectX SDK*"
      "C:/Program Files/Microsoft DirectX SDK*"
  )

  find_path(DX12_LIBRARY_DIR
    NAMES
      d3d12.lib
    PATH_SUFFIXES Lib/x64
    PATHS
      ${DX12_LIB_SEARCH_DIRS}
      "C:/Program Files (x86)/Microsoft DirectX SDK*/"
      "C:/Program Files/Microsoft DirectX SDK*/"
  )
  foreach(DX_LIB d3d12 d3dcompiler dxguid dxgi)
    find_library(DX12_${DX_LIB}_LIBRARY
      NAMES 
        ${DX_LIB}.lib
      PATHS
        ${DX12_LIBRARY_DIR}
    )
    list(APPEND DX12_LIBRARIES ${DX12_${DX_LIB}_LIBRARY})
  endforeach(DX_LIB)
endif ()

include(FindPackageHandleStandardArgs)

find_package_handle_standard_args(DX12 DEFAULT_MSG
  DX12_INCLUDE_DIR
  DX12_LIBRARY_DIR
  DX12_LIBRARIES
)

mark_as_advanced(
  DX12_INCLUDE_DIR
  DX12_LIBRARY_DIR
  DX12_LIBRARIES
)