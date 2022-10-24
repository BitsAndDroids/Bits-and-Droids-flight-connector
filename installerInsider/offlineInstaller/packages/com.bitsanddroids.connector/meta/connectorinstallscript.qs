function Component()
{
}
Component.prototype.createOperationsForArchive = function(archive)
{
    component.addOperation("Extract", archive, "@TargetDir@");
}

Component.prototype.createOperations = function()
{
    component.createOperations();

    if (systemInfo.productType === "windows") {
        component.addOperation("CreateShortcut", "@TargetDir@/BitsAndDroidsConnector.exe", "@StartMenuDir@/BitsAndDroidsConnector.lnk",
            "workingDirectory=@TargetDir@", "iconPath=@TargetDir@/BitsAndDroidsLogo.ico","description=Open the Bits and Droids Connector");
        component.addOperation("CreateShortcut", "@TargetDir@/bin/BitsAndDroidsConnector.exe", "@TargetDir@/BitsAndDroidsConnector.lnk",
            "workingDirectory=@TargetDir@", "iconPath=@TargetDir@/BitsAndDroidsLogo.ico","description=Open the Bits and Droids Connector");
    }
}
