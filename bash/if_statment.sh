var=$1

echo "input var = "$var

if [ $var = a ]; then
    echo a
elif [ $var = b ]; then
    echo b
elif [ $var = c ]; then
    echo c
else
    echo stocazzo
fi
