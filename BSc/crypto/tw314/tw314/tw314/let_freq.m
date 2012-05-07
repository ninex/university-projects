function [ret] = let_freq(lang,letter)
if strcmp(lang,'Afr') == 1
    freq = [897 129 2 579 1778 72 370 301 733 95 251 463 287 756 650 108 0 575 571 541 258 225 198 0 161 0];
elseif strcmp(lang,'Eng') == 1
    freq = [820 154 167 445 1260 265 177 825 630 28 76 403 252 639 810 134 3 537 622 871 269 101 249 6 247 10];
else
    disp('invalid language');
end
ret = freq(Zm(letter) + 1) * 10000;