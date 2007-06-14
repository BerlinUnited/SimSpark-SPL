#
# soccerbindings.rb sets up a mapping from input constants used by the
# inputserver to soccer command constants
#
inputServer = get('/sys/server/input');

if (inputServer == nil)
  print "(soccerbindings.rb) no InputServer present\n";
else
  print "(soccerbindings.rb) setting up bindings\n";

  if ($logPlayerMode == true)
    inputServer.bindCommand('p',      Command.Pause);
    inputServer.bindCommand('f',      Command.StepForward);
    inputServer.bindCommand('b',      Command.StepBackward);
    inputServer.bindCommand('l',      Command.BPlayback);
  else
    inputServer.bindCommand('k',      Command.KickOff);
  end
end
