" Vim syntax file
" Language: BrainFuck Advanced
" Maintainer: Quentin Bazin
" Latest Revision: 10 February 2012

if exists("b:current_syntax")
  finish
endif

" Keywords
syn keyword bfaKeywords $ & @

" Integer with - + or nothing in front
syn match bfaNumber '\d\+'
syn match bfaNumber '[-+]\d\+'

" Floating point number with decimal no E or e (+,-)
syn match bfaNumber '\d\+\.\d*'
syn match bfaNumber '[-+]\d\+\.\d*'

" Regions
syn region bfaCondition start="(" end=")" fold transparent
syn region bfaLoop start="[" end="]" fold transparent

