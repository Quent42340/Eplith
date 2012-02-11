" Mon fichier de types de fichiers
if exists("did_load_filetypes")
	finish
endif
augroup filetypedetect
	au! BufRead,BufNewFile *.ba			setfiletype bfa
augroup END
