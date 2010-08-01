BASEDIR=$PWD

sudo rm -rf /Library/Xsnmp

cd ~/Source/External/pcre-8.02
$BASEDIR/util/pcre_build.sh
cd $BASEDIR

make distclean
./bootstrap.sh && make && sudo make install

sudo cp launchd/com.xsnmp.xsnmp-agentx.plist /Library/Xsnmp/XsnmpAgentExtension.app/Resources
sudo cp -r packaging/XsnmpInstaller.pmdoc /Library/Xsnmp/XsnmpAgentExtension.app/Resources
sudo cp -r packaging/scripts /Library/Xsnmp/XsnmpAgentExtension.app/Resources
sudo cp /Library/Xsnmp/XsnmpAgentExtension.app/Resources/scripts/postinstall.template /Library/Xsnmp/XsnmpAgentExtension.app/Resources/scripts/postinstall
sudo cp packaging/Info.plist /Library/Xsnmp/XsnmpAgentExtension.app/Contents 

cd prefpane
xcodebuild -configuration Release clean
xcodebuild -configuration Release build
cd ..

sudo cp -r prefpane/build/release/Xsnmp.prefPane /Library/Xsnmp/XsnmpAgentExtension.app/Resources

/Developer/usr/bin/packagemaker --doc "/Library/Xsnmp/XsnmpAgentExtension.app/Resources/XsnmpInstaller.pmdoc" --out "packaging/Xsnmp Installer.pkg" --title "Xsnmp Installer"

#sudo rm -rf /Library/Xsnmp