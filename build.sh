
MAKE_COMMAND="make -C Generated/"

case $1 in
    "Trespass")
        eval $MAKE_COMMAND $1
        ;;
    "Clean")
        eval "$(rm -rf Build/Obj/Entropy/*)"
        ;;
esac


assemble_app_file()
{
    CMD_EXEC="./Build/Bin/Trespass/Debug/play.app"
    APP_NAME="Trespass"
    APP="./Build/Bin/Trespass/Debug/Mac/Trespass"
    mkdir -vp ${APP}.app/Contents/MacOS ${APP}.app/Contents/Resources ${APP}.app/Contents/Resources/lib ${APP}.app/Contents/MacOS/Tracing # Create the folders.
    PATH="$PATH:/usr/libexec" # Make sure PlistBuddy is in the PATH.

    # printf '#!/usr/bin/osascript\ntell application "Terminal"\n\tactivate\n\tdo script "top"\nend tell\n' > ${APP}.app/Contents/MacOS/${APP_NAME}

    echo "Clearing old app files..."
    rm ${APP}.app/Contents/MacOS/${APP_NAME}
    rm ${APP}.app/Contents/Resources/lib/libEntropy.dylib

    echo "Copying new app files..."
    cp ./Build/Bin/Entropy/Debug/libEntropy.dylib ${APP}.app/Contents/Resources/lib/libEntropy.dylib
    cp ${CMD_EXEC} ${APP}.app/Contents/MacOS/${APP_NAME}
    
    echo "Updating assets..."
    rsync -avzh -p -u ./Assets/ ./${APP}.app/Contents/Resources/Assets/

    chmod +x ${APP}.app/Contents/MacOS/${APP_NAME} # Sets the executable flag.
    
    echo ""
    echo "Building plist..."
    PlistBuddy ${APP}.app/Contents/Info.plist -c "add CFBundleDisplayName string ${APP_NAME}"
    PlistBuddy ${APP}.app/Contents/version.plist -c "add ProjectName string ${APP_NAME}"

    # Change dylib location
    install_name_tool -change @rpath/libEntropy.dylib  @executable_path/../Resources/lib/libEntropy.dylib ${APP}.app/Contents/MacOS/Trespass

    # cp Asse

    # find ${APP}.app # Verify the files.
    # open ${APP}.app # Run the app
}

assemble_app_file