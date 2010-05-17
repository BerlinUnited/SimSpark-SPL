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
    print "setting bindings for logplayer\n\n";
    inputServer.bindCommand('p',      Command.Pause);
    inputServer.bindCommand('f',      Command.StepForward);
    inputServer.bindCommand('b',      Command.StepBackward);
    inputServer.bindCommand('l',      Command.BPlayback);
    inputServer.bindCommand('1',      Command.CameraLeftGoal);
    inputServer.bindCommand('2',      Command.CameraLeftCorner);
    inputServer.bindCommand('3',      Command.CameraMiddleLeft);
    inputServer.bindCommand('4',      Command.CameraMiddle);
    inputServer.bindCommand('5',      Command.CameraMiddleRight);
    inputServer.bindCommand('6',      Command.CameraRightCorner);
    inputServer.bindCommand('7',      Command.CameraRightGoal);
  else
    print "setting bindings for online monitor\n\n";
    inputServer.bindCommand('k',      Command.KickOff);
    inputServer.bindCommand('j',      Command.KickOffRight);
    inputServer.bindCommand('b',      Command.DropBall);
    #inputServer.bindCommand('m',      Command.MoveAgent);
    #inputServer.bindCommand('n',      Command.ShootBall);
    #inputServer.bindCommand('x',      Command.MoveBall);
    inputServer.bindCommand('1',      Command.CameraLeftGoal);
    inputServer.bindCommand('2',      Command.CameraLeftCorner);
    inputServer.bindCommand('3',      Command.CameraMiddleLeft);
    inputServer.bindCommand('4',      Command.CameraMiddle);
    inputServer.bindCommand('5',      Command.CameraMiddleRight);
    inputServer.bindCommand('6',      Command.CameraRightCorner);
    inputServer.bindCommand('7',      Command.CameraRightGoal);
    inputServer.bindCommand('l',      Command.FreeKickLeft);
    inputServer.bindCommand('r',      Command.FreeKickRight);
    inputServer.bindCommand('lctrl y',      Command.NextMode);
    inputServer.bindCommand('n',      Command.SelectNextAgent);
  end
end
