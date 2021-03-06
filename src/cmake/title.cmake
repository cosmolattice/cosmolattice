if(NOT WIN32)
  string(ASCII 27 Esc)
  set(ColourReset "${Esc}[m")
  set(ColourBold  "${Esc}[1m")
  set(Red         "${Esc}[31m")
  set(Green       "${Esc}[32m")
  set(Yellow      "${Esc}[33m")
  set(Blue        "${Esc}[34m")
  set(Magenta     "${Esc}[35m")
  set(Cyan        "${Esc}[36m")
  set(White       "${Esc}[37m")
  set(BoldRed     "${Esc}[1;31m")
  set(BoldGreen   "${Esc}[1;32m")
  set(BoldYellow  "${Esc}[1;33m")
  set(BoldBlue    "${Esc}[1;34m")
  set(BoldMagenta "${Esc}[1;35m")
  set(BoldCyan    "${Esc}[1;36m")
  set(BoldWhite   "${Esc}[1;37m")
endif()

message("${BoldBlue}
MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM
MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMNKOKXNMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM
MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMNKOkkkxxkkkkkOKNMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM
MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMNKOkxddk0XWMMMMWN0kddxkOKWMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM
MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMNKOkkkkO0KXXXXXNNWNNXXXXXXX0Okkkk0XNMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM
MMMMMMMMMMMMMMMMMMMMMMMMMMNXOkxdddOXWMMMMMMWXKOO0OO0KNWMMMMMMWKkddxkk0XWMMMMMMMMMMMMMMMMMMMMMMMMMMMM
MMMMMMMMMMMMMMMMMMMMMNKOkkkkk0XNNXXXXXXXXXXXXXNWWWNNXXXXXXXXXXXXNNNX0kkkkk0XWMMMMMMMMMMMMMMMMMMMMMMM
MMMMMMMMMMMMMMMMWXOkkxxdx0XWWMMMMWNXK00000KNWWMMMMMMWNXK00000KNWWMMMMMWXOxdxkkk0XWMMMMMMMMMMMMMMMMMM
MMMMMMMMMMMMMKxdxkk0XWMWNXXXKKKKKXXXNWMMMWNXXXKKKKKKXXXNWMMMWNXXXKKKKXXXNWWMWXOkxdokXMMMMMMMMMMMMMMM
MMMMMMMMMMMMWd;oxkO0NWMWNNXXKKKKKXXXNWMMMWNXXXK000KKXXNWWMMMWNXXKKK0KXXXNWMWNKOkkdl:kMMMMMMMMMMMMMMM
MMMMMMMMMMMMWddNNKOkkxxdk0XWMMMMMWNXK00000KXNWMMMMMWWNXK0000KXNWWMMMMMNKkddxxkOKNWXokMMMMMMMMMMMMMMM
MMMMMMMMMMMMWddWMMMMMNKOkkkkO0XWNNXXXXXXXXXXXXNWWWNNXXXXXXXXXXXXNNNKOkkkkO0XWMMMMMXokMMMMMMMMMMMMMMM
MMMMMMMMMMMMWddWMMMMMMMMMMNKOkkxddOXWMMMMMMWX0OO0OO0KNWMMMMMWN0xddxkOKNWMMMMMMMMMMXokMMMMMMMMMMMMMMM
MMMMMMMMMMMMWddWMMMMMMMMMMMMMMWNKOkkkkO0KXXXXXNNWNNXXXXXK0OkkkkOKNWMMMMMMMMMMMMMMMXokMMMMMMMMMMMMMMM
MMMMMMMMMMMMWddWMMMMMMMMMMMMMMMMMMMMNKOkdddk0NWMMWWNKOxddxOKNWMMMMMMMMMMMMMMMMMMMMXokMMMMMMMMMMMMMMM
MMMMMMMMMMMMWddWMMMMMMMMMMMMMMMMMMMMMMMMWNKOkkkxxxkkkOKNWMMMMMMMMMMMMMMMMMMMMMMMMMXokMMMMMMMMMMMMMMM
MMMMMMMMMMMMWddWMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMNxl0WWMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMXokMMMMMMMMMMMMMMM
MMMMMMMMMMMMWddWMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMOoKMMMMMMMMMMMMMMMMMMMWX0NMMMMMMMMMXokMMMMMMMMMMMMMMM
MMMMMMMMMMMMWddWMMMMMMMMMMMMMMMMMMMMMWWMMMMMMMMOoKMMMMMMMMMMMMMMMMMMNd..lNMMMMMMMMXokMMMMMMMMMMMMMMM
MMMMMMMMMMMMWddWMMMMMMMMMMMMMMMMMWXkl:;oKMMMMMMOoKMMMMMMMMMMMMMMMMMNo.. ;XMMMMMMMMXokMMMMMMMMMMMMMMM
MMMMMMMMMMMMWddWMMMMMMMMMMMMMMMWKl,,ox;.lNMMMMMOoKMMMMMMMMMMMMMMMMWx'lc ;XMMMMMMMMXokMMMMMMMMMMMMMMM
MMMMMMMMMMMMWddWMMMMMMMMMMMMMMXo..oXMK,.oWMMMMMOoKMMMMMMMMMMMMMMMM0,cO; lNMMMMMMMMXokMMMMMMMMMMMMMMM
MMMMMMMMMMMMWddWMMMMMMMMMMMMWO,.;OWMK: ,0MMMMMMOoKMMMMMMMMMMMMMMMNl,OO. cNMMMMMMMMXokMMMMMMMMMMMMMMM
MMMMMMMMMMMMWddWMMMMMMMMMMMNd..lXWXk; 'OWMMMMMMOoKMMMMMMMMMMMMMMMO,lNk..dWMMMMMMMMXokMMMMMMMMMMMMMMM
MMMMMMMMMMMMWddWMMMMMMMMMMXl..oNNd.  ;0WMMMMMMMOoKMMMMMMMMMMMMMMNc,0MKoxNMMMMMMMMMXokMMMMMMMMMMMMMMM
MMMMMMMMMMMMWddWMMMMMMMMMNl .lNNl. .lXMMMMMMMMMOoKMMMMMMMMMMMMMMO,lNMMWMMMMMMMMMMMXokMMMMMMMMMMMMMMM
MMMMMMMMMMMMWddWMMMMMMMMWd. :XMKc'c0WMMNKXMMMMMOoKMMMMMMMMMMMMMNl,OMMMMMWWNXNMMMMMXokMMMMMMMMMMMMMMM
MMMMMMMMMMMMWddWMMMMMMMM0' .xMMMNNWMMW0dkNMMMMMOoKMMMMMMMMMMMMM0,,ddolc:;,';kWMMMMXokMMMMMMMMMMMMMMM
MMMMMMMMMMMMWddWMMMMMMMMx. .OMMMMMMWKdoOWMMMMMMOoKMMMMMMMMMMNOl'          .lNMMMMMXokMMMMMMMMMMMMMMM
MMMMMMMMMMMMWddWMMMMMMMMk.  lXMMWXkl:dXMMMMMMMMOoKMMMMMMMMM0:   ..,;:lodxk0NMMMMMMXokMMMMMMMMMMMMMMM
MMMMMMMMMMMMWddWMMMMMMMMXc   '::;,;oKWMMMMMMMMMOoKMMMMMMMMNl .:x0XNWMMMMMMMMMMMMMMXokMMMMMMMMMMMMMMM
MMMMMMMMMMMMWddWMMMMMMMMMNx:,',:okNMMMMMMMMMMMMOoKMMMMMMMMNxd0WMMMMMMMMMMMMMMMMMMMXokMMMMMMMMMMMMMMM
MMMMMMMMMMMMWddWMMMMMMMMMMMWWNWMMMMMMMMMMMMMMMMOoKMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMXokMMMMMMMMMMMMMMM
MMMMMMMMMMMMWddWMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMOoKMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMXokMMMMMMMMMMMMMMM
MMMMMMMMMMMMWxdWMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMOoKMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMXokMMMMMMMMMMMMMMM
MMMMMMMMMMMMWkokKNMMMMMMMMMMMMMMMMMMMMMMMMMMMMMOoKMMMMMMMMMMMMMMMMMMMMMMMMMMMMMWX0xo0MMMMMMMMMMMMMMM
MMMMMMMMMMMMMWKOkkkkOKNWMMMMMMMMMMMMMMMMMMMMMMMOoKMMMMMMMMMMMMMMMMMMMMMMMMWXOkkkkk0XWMMMMMMMMMMMMMMM
MMMMMMMMMMMMMMMMMWNKOkkkkOKNWMMMMMMMMMMMMMMMMMMOoKMMMMMMMMMMMMMMMMMMMNX0kkkkk0XWMMMMMMMMMMMMMMMMMMMM
MMMMMMMMMMMMMMMMMMMMMMWNKOkkkkOKNWMMMMMMMMMMMMMOoKMMMMMMMMMMMMMMWX0kkkkk0XWMMMMMMMMMMMMMMMMMMMMMMMMM
MMMMMMMMMMMMMMMMMMMMMMMMMMMWNKOkkkkOKNWMMMMMMMMOoKMMMMMMMMMWX0kkkkk0XWMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM
MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMWNKOkkkkOKNWMMMOoKMMMMWX0kkkkk0XWMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM
MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMWNKOkkkkOKxlkK0kkkkk0XWMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM
MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMWNKOkocdkO0XWMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM
MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMWWWMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM
MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM

   a88888b.                                       dP                   dP     dP   oo
  d8'   `88                                       88                   88     88
  88        .d8888b. .d8888b. 88d8b.d8b. .d8888b. 88        .d8888b. d8888P d8888P dP .d8888b. .d8888b.
  88        88'  `88 Y8ooooo. 88'`88'`88 88'  `88 88        88'  `88   88     88   88 88'  `88 88ooood8
  Y8.   .88 88.  .88       88 88  88  88 88.  .88 88        88.  .88   88     88   88 88.  ... 88.  ...
   Y88888P' `88888P' `88888P' dP  dP  dP `88888P' 88888888P `88888P8   dP     dP   dP `88888P' `88888P'
  oooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooo

  ${ColourReset}")

#: Nancyj-Underlined
# ascii logo from : https://www.ascii-art-generator.org/