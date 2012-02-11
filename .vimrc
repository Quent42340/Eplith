" My vimrc file.

" When started as "evim", evim.vim will already have done these settings.
if v:progname =~? "evim"
  finish
endif

" Use Vim settings, rather than Vi settings (much better!).
" This must be first, because it changes other options as a side effect.
set nocompatible

filetype off                   " required!

set rtp+=~/.vim/bundle/vundle/
call vundle#rc()

" let Vundle manage Vundle
" required! 
Bundle 'gmarik/vundle'

" My Bundles here:
"
" original repos on github
Bundle 'tpope/vim-fugitive'
Bundle 'Lokaltog/vim-easymotion'
Bundle 'rstacruz/sparkup', {'rtp': 'vim/'}
" vim-scripts repos
Bundle 'L9'
Bundle 'FuzzyFinder'
Bundle 'rails.vim'
Bundle 'AutoClose'
Bundle 'c.vim'
Bundle 'Intelligent-Tags'
Bundle 'VimCalc'
Bundle 'Tab-Manager'
Bundle 'Tab-Menu'
Bundle 'project.tar.gz'
" non github repos
Bundle 'git://git.wincent.com/command-t.git'
" ...

" allow backspacing over everything in insert mode
set backspace=indent,eol,start

if has("vms")
  set nobackup		" do not keep a backup file, use versions instead
else
  set backup		" keep a backup file
endif
set history=50		" keep 50 lines of command line history
set ruler		" show the cursor position all the time
set showcmd		" display incomplete commands
set incsearch		" do incremental searching
set number
set smartcase
set smartindent
set tabstop=4
set softtabstop=4
set shiftwidth=4
set wrap!

colorscheme desert

" To save, ctrl-s.
nmap <C-S> :w<CR>
imap <C-S> <Esc>:w<CR>a

" To undo, ctrl-z
imap <C-Z> <Esc>u<CR>a

" My dev shortcuts..
nmap <C-F2> :make<CR>
imap <C-F2> <Esc>:make<CR>

nmap <C-F3> :make run<CR>
imap <C-F3> <Esc>:make run<CR>

nmap <C-F4> :make debug<CR>
imap <C-F4> <Esc>:make debug<CR>

nmap <C-F5> :copen<CR>
imap <C-F5> <Esc>:copen<CR>a

nmap <C-F6> :cclose<CR>
imap <C-F6> <Esc>:cclose<CR>a

nmap <C-F7> :Project *.vimproj<CR>

" Fix indent
inoremap <CR> <CR>x<BS>
map o ox<BS>
map O Ox<BS>

" Perso commands
map ,y "+y
map ,yy "+yy
map ,p "+gP
map ,c "+x
map ,cl "+dd
map ,sa ggVG
map ,d "zyy"zp
map ; a;<Esc>

" Cut Copy Paste
nmap <C-X> ,c
vmap <C-X> ,c
imap <C-X> <Esc>,ca

nmap <C-C> ,y
vmap <C-C> ,y
imap <C-C> <Esc>,ya

nmap <C-P> ,p
nmap <C-P> ,p
imap <C-P> <Esc>,pa

nmap <C-D> ,d
imap <C-D> <Esc>,da

" Git commands
map ,ga :!git add .<cr>
map ,gs :!git status<cr>
map ,gc :!git commit -am “”<left>
map ,gC :!git add .<cr>:!git commit -am “”<left>
map ,gd :!git diff<cr>
map ,gD :!git diff —cached<cr>
map ,gp :!git push<cr>
map ,gP :!git pull<cr>
map ,gb :!git branch<cr>
map ,gB :!git branch
map ,gh :!git checkout
map ,gi :!git init<cr>
map ,gt :!tig<cr>
map ,gl :!git log<cr>

" To know if tab is used and if there is some trailing spaces
"set list
"set lcs:tab:>_,extends:>,precedes:<

" Raccourcis pour se déplacer entre les onglets.
" Alt-j pour déplacer l'onglet vers la gauche
noremap <A-J> gT
" Alt-k pour déplacer l'onglet vers la droite
noremap <A-K> gt

" For Win32 GUI: remove 't' flag from 'guioptions': no tearoff menu entries
" let &guioptions = substitute(&guioptions, "t", "", "g")

" Don't use Ex mode, use Q for formatting
map Q gq

" CTRL-U in insert mode deletes a lot.  Use CTRL-G u to first break undo,
" so that you can undo CTRL-U after inserting a line break.
inoremap <C-U> <C-G>u<C-U>

" In many terminal emulators the mouse works just fine, thus enable it.
if has('mouse')
  set mouse=a
endif

" Switch syntax highlighting on, when the terminal has colors
" Also switch on highlighting the last used search pattern.
if &t_Co > 2 || has("gui_running")
  syntax on
  set hlsearch
endif

" Only do this part when compiled with support for autocommands.
if has("autocmd")

  " Enable file type detection.
  " Use the default filetype settings, so that mail gets 'tw' set to 72,
  " 'cindent' is on in C files, etc.
  " Also load indent files, to automatically do language-dependent indenting.
  filetype plugin indent on

  " Put these in an autocmd group, so that we can delete them easily.
  augroup vimrcEx
  au!

  " For all text files set 'textwidth' to 78 characters.
  autocmd FileType text setlocal textwidth=78

  " When editing a file, always jump to the last known cursor position.
  " Don't do it when the position is invalid or when inside an event handler
  " (happens when dropping a file on gvim).
  " Also don't do it when the mark is in the first line, that is the default
  " position when opening a file.
  autocmd BufReadPost *
    \ if line("'\"") > 1 && line("'\"") <= line("$") |
    \   exe "normal! g`\"" |
    \ endif

  augroup END

else

  set autoindent		" always set autoindenting on

endif " has("autocmd")

" Convenient command to see the difference between the current buffer and the
" file it was loaded from, thus the changes you made.
" Only define it when not defined already.
if !exists(":DiffOrig")
  command DiffOrig vert new | set bt=nofile | r # | 0d_ | diffthis
		  \ | wincmd p | diffthis
endif

if has("gui_running")
  " Don't display the menu or toolbar. Just the screen.
  " set guioptions-=m
  set guioptions-=T
endif

" configure tags - add additional tags here or comment out not-used ones
set tags+=~/.vim/tags/libnds

" build tags of your own project with CTRL+F12
"map <C-F12> :!ctags -R --c++-kinds=+p --fields=+iaS --extra=+q .<CR>
noremap <F12> :!ctags -R --c++-kinds=+p --fields=+iaS --extra=+q .<cr>
inoremap <F12> <Esc>:!ctags -R --c++-kinds=+p --fields=+iaS --extra=+q .<cr>

" OmniCppComplete
let OmniCpp_NamespaceSearch = 1
let OmniCpp_GlobalScopeSearch = 1
let OmniCpp_ShowAccess = 1
let OmniCpp_MayCompleteDot = 1
let OmniCpp_MayCompleteArrow = 1
let OmniCpp_MayCompleteScope = 1
let OmniCpp_DefaultNamespaces = ["std", "_GLIBCXX_STD"]

" automatically open and close the popup menu / preview window
au CursorMovedI,InsertLeave * if pumvisible() == 0|silent! pclose|endif
set completeopt=menuone,menu,longest,preview
