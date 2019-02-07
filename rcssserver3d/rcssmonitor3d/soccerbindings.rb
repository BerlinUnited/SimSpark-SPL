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
    inputServer.bindCommand('m',      Command.MoveAgent);
    #inputServer.bindCommand('n',      Command.ShootBall);
    #inputServer.bindCommand('x',      Command.MoveBall);
    
    #inputServer.bindCommand('1',      Command.CameraLeftGoal);
    #inputServer.bindCommand('2',      Command.CameraLeftCorner);
    #inputServer.bindCommand('3',      Command.CameraMiddleLeft);
    #inputServer.bindCommand('4',      Command.CameraMiddle);
    #inputServer.bindCommand('5',      Command.CameraMiddleRight);
    #inputServer.bindCommand('6',      Command.CameraRightCorner);
    #inputServer.bindCommand('7',      Command.CameraRightGoal);
    inputServer.bindCommand('1',      Command.One);
    inputServer.bindCommand('2',      Command.Two);
    inputServer.bindCommand('3',      Command.Three);
    inputServer.bindCommand('4',      Command.Four);
    inputServer.bindCommand('5',      Command.Five);
    inputServer.bindCommand('6',      Command.Six);
    inputServer.bindCommand('7',      Command.Seven);
    inputServer.bindCommand('8',      Command.Eight);
    inputServer.bindCommand('9',      Command.Nine);
    inputServer.bindCommand('0',      Command.Zero);

    inputServer.bindCommand('l',      Command.Left);
    inputServer.bindCommand('r',      Command.Right);
    inputServer.bindCommand('shift l',      Command.DirectLeft);
    inputServer.bindCommand('shift r',      Command.DirectRight);
    inputServer.bindCommand('lctrl s',      Command.PlayerSelectMode);
    inputServer.bindCommand('n',      Command.SelectNextAgent);
    inputServer.bindCommand('e',      Command.ResetSelection);
    inputServer.bindCommand('x',      Command.KillSelection);
    inputServer.bindCommand('p',      Command.ReposSelection);
  end
end
