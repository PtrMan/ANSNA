Str=""
for i in ./src/*.c
do
    Str=$Str" ${i%.c}.c"
done
echo $Str
g++ -g3 -o0  -Wall -Wmissing-prototypes -Wmissing-declarations  -Werror=missing-prototypes  $Str -lm -oANSNA 
