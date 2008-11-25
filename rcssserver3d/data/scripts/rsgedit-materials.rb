# 
# rsgedit-materials.rb -- create rsgedit specific materials
#

#----------------------------------------------------------------
# special purpose materials
#----------------------------------------------------------------

# highlighting of selected nodes
material = sparkCreate('kerosin/MaterialSolid', $serverPath+'material/matSelected');
material.setDiffuse(0.9,0.1,0.9,0.9)
