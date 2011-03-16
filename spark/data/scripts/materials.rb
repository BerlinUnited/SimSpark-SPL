#----------------------------------------------------------------
# general colored materials
#----------------------------------------------------------------

sparkGetMaterialServer()

# rainbow color
material = sparkCreate('kerosin/MaterialSolid', $serverPath+'material/matYellow');
material.setDiffuse(1.0,1.0,0.0,1.0)
material.setAmbient(0.2,0.2,0.0,1.0)

material = sparkCreate('kerosin/MaterialSolid', $serverPath+'material/matOrange');
material.setDiffuse(1.0,0.03,0.0,1.0)
material.setAmbient(0.0,0.0,0.0,0.0)
material.setEmission(0.0,0.0,0.0,0.0)
material.setSpecular(0.0,0.0,0.0,0.0)

material = sparkCreate('kerosin/MaterialSolid', $serverPath+'material/matRed');
material.setDiffuse(1.0,0.0,0.0,1.0)
material.setAmbient(1.0,0.0,0.0,1.0)

material = sparkCreate('kerosin/MaterialSolid', $serverPath+'material/matRedGlow');
material.setDiffuse(1.0,0.0,0.0,1.0)
material.setEmission(0.5,0.0,0.0,1.0)

material = sparkCreate('kerosin/MaterialSolid', $serverPath+'material/matMagenta');
material.setDiffuse(1.0,0.0,0.75,1.0)

material = sparkCreate('kerosin/MaterialSolid', $serverPath+'material/matBlue');
material.setDiffuse(0.0,0.0,1.0,1.0)
material.setAmbient(0.0,0.0,0.2,1.0)

material = sparkCreate('kerosin/MaterialSolid', $serverPath+'material/matLightBlue');
material.setDiffuse(0.0,0.75,1.0,1.0)

material = sparkCreate('kerosin/MaterialSolid', $serverPath+'material/matGreen');
material.setDiffuse(0.1,0.6,0.1,1.0)
material.setAmbient(0.1,0.3,0.1,1.0)

# selection marker
material = sparkCreate('kerosin/MaterialSolid', $serverPath+'material/matSelect');
material.setDiffuse(1.0,0.0,0.0,0.5)
material.setAmbient(1.0,0.0,0.0,0.5)
material.setSpecular(1.0,0.2,0.2,0.5);

# shades of grey
material = sparkCreate('kerosin/MaterialSolid', $serverPath+'material/matWhite');
material.setDiffuse(1.0,1.0,1.0,1.0)

material = sparkCreate('kerosin/MaterialSolid', $serverPath+'material/matGrey');
material.setDiffuse(0.2,0.2,0.2,1.0)

material = sparkCreate('kerosin/MaterialSolid', $serverPath+'material/matDarkGrey');
material.setDiffuse(0.05,0.05,0.05,1.0)
