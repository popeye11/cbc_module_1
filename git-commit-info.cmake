## This file provides CMake functions to obtain the hash of the last Git commit
## as well as information concerning the status of the workarea.
##
## See the end of this file for some sample usage.

find_package(Git QUIET)

## @brief return the hash (SHA1) for the last commit of the Git repository on
## directory DIR.
##
## @param dir directory containing Git repository.
## @param var on output this string variable contains the value of the hash.
function(git_commit_hash dir var)
    if (Git_FOUND)
        execute_process(
            COMMAND ${GIT_EXECUTABLE} rev-parse --short HEAD
            WORKING_DIRECTORY "${dir}"
            OUTPUT_VARIABLE _git_hash
            OUTPUT_STRIP_TRAILING_WHITESPACE
            ERROR_QUIET
            )
    else()
        message(WARNING "Git executable not found")
        set(_git_hash "0000000")
    endif()

    set(${var} "${_git_hash}" PARENT_SCOPE)
endfunction()

## @brief return a Boolean value indicating whether the Git workarea on
## directory DIR contains any modified files.
##
## @param dir directory containing Git workarea.
## @param var on output this Boolean variable contains the result of this function.
function(git_modified dir var)
    if (Git_FOUND)
        execute_process(
            COMMAND ${GIT_EXECUTABLE} diff --name-only --exit-code HEAD
            WORKING_DIRECTORY "${dir}"
            OUTPUT_VARIABLE _git_modified_files
            RESULT_VARIABLE _git_diff_rc
            ERROR_QUIET
            )
        if ("${_git_diff_rc}" EQUAL 1)
            set(_modified TRUE)
            message(STATUS "MODIFIED files in repository:\n"  ${_git_modified_files})
        else()
            set(_modified FALSE)
        endif()
    else()
        message(WARNING "Git executable not found")
        set(_modified FALSE)
    endif()


    set(${var} "${_modified}" PARENT_SCOPE)
endfunction()

## @brief return a Boolean value indicating whether the Git workarea on
## directory DIR contains any untracked files.
##
## @param dir directory containing Git workarea.
## @param var on output this Boolean variable contains the result of this function.
function(git_has_untracked_files dir var)
    if (Git_FOUND)
        execute_process(
            COMMAND ${GIT_EXECUTABLE} ls-files --other --directory --no-empty-directory --exclude-standard
            WORKING_DIRECTORY "${dir}"
            OUTPUT_VARIABLE _git_untracked_files
            OUTPUT_STRIP_TRAILING_WHITESPACE
            ERROR_QUIET
            )

        if ("${_git_untracked_files}" STREQUAL "")
            set(_git_untracked FALSE)
        else()
            set(_git_untracked TRUE)
            message(STATUS "UNTRACKED files in repository:\n"  ${_git_untracked_files})
        endif()
    else()
        message(WARNING "Git executable not found")
        set(_git_untracked FALSE)
    endif()

    set(${var} "${_git_untracked}" PARENT_SCOPE)
endfunction()

## @brief return a Boolean value indicating whether the Git workarea on
## directory DIR is "dirty" (contains modified files or untracked files).
##
## @param dir directory containing Git workarea.
## @param var on output this Boolean variable contains the result of this function.
function(git_is_project_dirty dir var)
    if (Git_FOUND)
        git_modified("${dir}" _git_modified)
        git_has_untracked_files("${dir}" _git_has_untracked)

        if (_git_modified OR _git_has_untracked)
            set(_git_dirty TRUE)
        else()
            set(_git_dirty FALSE)
        endif()
    else()
        message(WARNING "Git executable not found")
        set(_git_dirty FALSE)
    endif()

    set(${var} "${_git_dirty}" PARENT_SCOPE)
endfunction()

## @brief return the hash (SHA1) for the last commit of the Git repository on
## directory DIR.  If the workarea is "dirty", append an asterisk to the hash.
##
## @param dir directory containing Git repository.
## @param var on output this string variable contains the result of this function.
function(git_decorated_commit_hash dir var)
    git_commit_hash("${dir}" _hash)
    git_is_project_dirty("${dir}" _dirty)

    if (_dirty)
        string(CONCAT _decorated "${_hash}" "*")
    else()
        set(_decorated "${_hash}")
    endif()

    set(${var} "${_decorated}" PARENT_SCOPE)
endfunction()

# git_modified("." modified)
# message(STATUS "is modified: " ${modified})

# git_has_untracked_files("." untracked)
# message(STATUS "has untracked files: " ${untracked})

# git_is_project_dirty("." dirty)
# message(STATUS "is dirty: " ${dirty})

# git_commit_hash("." csum)
# message(STATUS "commit hash: " ${csum})

# git_decorated_commit_hash("." _decorated)
# message(STATUS "decorated commit hash: " ${_decorated})
