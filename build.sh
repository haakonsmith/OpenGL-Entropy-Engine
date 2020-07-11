
# otool -L
assemble_app_file()
{
    GLFW_FILE="libglfw.3.dylib"
    CMD_EXEC="./Build/Bin/Trespass/$1/play.app"

    echo ${CMD_EXEC}

    APP_NAME="Trespass"
    APP="./Build/Bin/Trespass/$1/Mac/Trespass"
    mkdir -vp ${APP}.app/Contents/MacOS ${APP}.app/Contents/Resources ${APP}.app/Contents/Resources/lib ${APP}.app/Contents/MacOS/Tracing # Create the folders.
    PATH="$PATH:/usr/libexec" # Make sure PlistBuddy is in the PATH.

    # printf '#!/usr/bin/osascript\ntell application "Terminal"\n\tactivate\n\tdo script "top"\nend tell\n' > ${APP}.app/Contents/MacOS/${APP_NAME}

    echo "Clearing old app files..."
    rm ${APP}.app/Contents/MacOS/${APP_NAME}
    rm ${APP}.app/Contents/Resources/lib/libEntropy.dylib

    echo "Copying new app files..."
    eval cp ./Build/Bin/Entropy/$1/libEntropy.dylib ${APP}.app/Contents/Resources/lib/libEntropy.dylib
    cp ${CMD_EXEC} ${APP}.app/Contents/MacOS/${APP_NAME}
    # cp Libraries/GLFW/Lib/${GLFW_FILE} ${APP}.app/Contents/MacOS/${APP_NAME}
    
    echo "Updating assets..."
    rsync -t -v -h -r -P ./Assets/ ./${APP}.app/Contents/Resources/Assets/

    chmod +x ${APP}.app/Contents/MacOS/${APP_NAME} # Sets the executable flag.
    
    echo ""
    echo "Building plist..."
    PlistBuddy ${APP}.app/Contents/Info.plist -c "add CFBundleDisplayName string ${APP_NAME}"
    PlistBuddy ${APP}.app/Contents/version.plist -c "add ProjectName string ${APP_NAME}"

    # Change dylib location
    install_name_tool -change @rpath/libEntropy.dylib  @executable_path/../Resources/lib/libEntropy.dylib ${APP}.app/Contents/MacOS/Trespass
    install_name_tool -change /usr/local/opt/glfw/lib/libglfw.3.dylib  @executable_path/../Resources/lib/${GLFW_FILE} ${APP}.app/Contents/MacOS/Trespass

    # cp Asse

    # find ${APP}.app # Verify the files.
    # open ${APP}.app # Run the app
}


MAKE_COMMAND="make -C Generated/"

case $1 in
    "Trespass")
        case $2 in
            $"release")
                echo "Args: ${@:3}"
                eval $MAKE_COMMAND $1 "${@:3}" "config=release"
                assemble_app_file Release
            ;;
            $"debug")
                eval $MAKE_COMMAND $1 "${@:3}" "config=debug"
                assemble_app_file Debug
            ;;
            $"analyse")
                eval "scan-build ${MAKE_COMMAND} ${1} ${@:3} config=debug"
            ;;
        esac
        ;;
    "Clean")
        eval "$(rm -rf Build/Obj/Entropy/*)"
        ;;
esac



