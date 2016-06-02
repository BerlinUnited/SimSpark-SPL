#
# internalsoccerbindings.rb sets up a mapping from input constants used by the
# inputserver to soccer command constants
#
bfilename = "internalsoccerbindings.rb"
bprefix = "("+bfilename+") "

inputServer = get('/sys/server/input');

if (inputServer == nil)
  logNormal (bprefix + "No InputServer present\n")
else
  if ($logPlayerMode == true)
    logNormal (bprefix + "Setting up bindings for log player mode.\n")
    inputServer.bindCommand('p',      Command.Pause);
    inputServer.bindCommand('f',      Command.StepForward);
    inputServer.bindCommand('b',      Command.StepBackward);
    inputServer.bindCommand('l',      Command.BPlayback);
  else
    logNormal (bprefix + "Setting up bindings.\n")
    inputServer.bindCommand('k',      Command.KickOff);
    inputServer.bindCommand('b',      Command.DropBall);
    inputServer.bindCommand('c',      Command.KillAgentLeft);
    inputServer.bindCommand('m',      Command.KillAgentRight);
    inputServer.bindCommand('n',      Command.ShootBall);
    inputServer.bindCommand('x',      Command.MoveBall);
    inputServer.bindCommand('1',      Command.CameraLeftGoal);
    inputServer.bindCommand('2',      Command.CameraLeftCorner);
    inputServer.bindCommand('3',      Command.CameraMiddleLeft);
    inputServer.bindCommand('4',      Command.CameraMiddle);
    inputServer.bindCommand('5',      Command.CameraMiddleRight);
    inputServer.bindCommand('6',      Command.CameraRightCorner);
    inputServer.bindCommand('7',      Command.CameraRightGoal);
    inputServer.bindCommand('l',      Command.FreeKickLeft);
    inputServer.bindCommand('r',      Command.FreeKickRight);
    
    inputServer.bindCommand(',',      Command.NextCamera);
    inputServer.bindCommand('.',      Command.PreviousCamera);
  end
end
