
MAKE_COMMAND="make -C Generated/"

case $1 in
    "Trespass")
        eval $MAKE_COMMAND $1
    "Clean")
        eval "$(rm -rf Build/Obj/Entropy/*)"
esac