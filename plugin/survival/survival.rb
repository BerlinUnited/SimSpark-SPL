#
# Import classes of this simulation description
#
importBundle('survival');

# the constructed scene
#
#
# @scene/
#       _control/                (SurvivalControlAspect)
#       _world/                  (kerosin/World)
#       _space/                  (kerosin/Space)

#       camera/                  (kerosin/Transform)
#               camera/          (kerosin/Camera)
#              _physics/         (kerosin/Body)
#                       fps/     (kerosin/FPSController)
#              _light            (kerosin/Light)
#              _geometry         (kerosin/SphereCollider)

#       arena/                   (kerosin/Transform)
#               _visual          (kerosin/StaticMesh)
#               pc               (kerosin/PlaneCollider)
#               w1               (kerosin/PlaneCollider)
#               w2               (kerosin/PlaneCollider)
#               w3               (kerosin/PlaneCollider)
#               w4               (kerosin/PlaneCollider)

#       light0/                  (kerosin/Transfom)
#               _light           (kerosin/Light)

#       <agentName>              (kerosin/Transform)
#               _visual          (kerosin/StaticMesh)
#               _physics         (kerosin/Body)
#               _geometry        (kerosin/SphereCollider)

#       food                     (kerosin/Transform)
#               _visual          (kerosin/StaticMesh)

class SurvivalInitializer
  # the constructor
  def initialize()	
    @scene = '/usr/scene';
    createControl();
    createWorld();
    createSpace();
    createArena();
    createLight();
    createAgents();
    createCamera();
  end

  # create the control aspect
  def createControl()
    cd (@scene);
    controlAspect = new ('SurvivalControlAspect', @scene+'/_control');
  end

  # create world
  def createWorld()
    cd (@scene);
    world = new ('kerosin/World', '_world');
    world.setGravity(0.0, -9.81, 0.0);
  end

  #create space
  def createSpace()
    cd (@scene)
    new ('kerosin/Space', '_space');
  end

  # setup camera
  def createCamera()
    cd (@scene)

    trans = new ('kerosin/Transform', 'camera');
    trans.setLocalPos(0.0, 0.0, 10.0);
    baseCamera = @scene + '/camera';

    cd(baseCamera)
    new ('kerosin/Camera', 'camera');

    cd (baseCamera)
    body = new ('kerosin/Body', '_physics');
    body.useGravity(false);
    new ('kerosin/FPSController', 'fps');

    cd (baseCamera)
    light = new ('kerosin/Light', '_light');
    light.setRadius(50.0);
    light.setDiffuseColor(1.0, 1.0, 1.0);

    cd (baseCamera)
    collider = new ('kerosin/SphereCollider', '_geometry');
    collider.setRadius(2.0);
  end

  # setup arena
  def createArena()
    cd (@scene);

    trans = new ('kerosin/Transform', 'arena');
    trans.setLocalPos(0.0, 0.0, 0.0);
    baseArena = @scene +  '/arena';

    cd (baseArena)
    mesh = new ('kerosin/StaticMesh', '_visual');
    mesh.load('model/arena.void');

    # floor collider
    cd (baseArena)
    pc = new ('kerosin/PlaneCollider', 'pc');
    pc.setParams(0.0, 1.0 ,0.0, 0.0); 
  
    # walls
    cd (baseArena)
    w1 = new ('kerosin/PlaneCollider', 'w1');
    w1.setParams(-1.0, 0.0, 0.0, -25.0);

    cd (baseArena)
    w2 = new ('kerosin/PlaneCollider', 'w2');
    w2.setParams(1.0, 0.0, 0.0, -25.0);

    cd (baseArena)
    w3 = new ('kerosin/PlaneCollider', 'w3');
    w3.setParams(0.0, 0.0, -1.0, -25.0);

    cd (baseArena)
    w4 = new ('kerosin/PlaneCollider', 'w4');
    w4.setParams(0.0, 0.0, 1.0, -25.0);
  end

  # setup global lighting
  def createLight()
    cd (@scene)

    trans = new ('kerosin/Transform', 'light0');
    trans.setLocalPos(0.0, 15.0, 0.0);

    light = new ('kerosin/Light', '_light');
    light.setRadius(80.0);
    light.setDiffuseColor(1.0, 1.0, 1.0);
  end

  # setup an agent
  def createAgent(path, name, model, radius, maxSpeed)
    cd (path);

    trans = new ('kerosin/Transform', name);
    trans.setLocalPos(-12.5, 20.0, -12.5);
    baseAgent = path + '/' + name;

    cd (baseAgent)
    visual = new ('kerosin/StaticMesh', '_visual');
    visual.load(model);

    cd (baseAgent)
    physics = new ('kerosin/Body', '_physics');
    physics.setSphere(1.0, radius);
    physics.setMass(0.5);
    physics.setMaxSpeed(maxSpeed);

    cd (baseAgent)
    geometry = new ('kerosin/SphereCollider', '_geometry');
    geometry.setRadius(radius);

    #cd baseAgent
    #light = new ('kerosin/Light', '../_light');
    #light.setRadius(5.0);
    #light.setDiffuseColor(0.0, 1.0, 0.0);

    cd (baseAgent)
    agent = new ('SurvivalAgentAspect', '_agent');
    #agent.setWanderingStrength(5.0);
    #agent.setWanderingRate(2.0);
  end

  # setup food
  def createFood()
    cd (@scene);

    trans = new ('kerosin/Transform', 'food');
    trans.setLocalPos(0.0, 1.0, 0.0);
    baseFood = @scene + '/food';

    cd (baseFood);
    visual = new ('kerosin/StaticMesh', '_visual');
    visual.load('model/food.void');

    cd (baseFood);
    light = new ('kerosin/Light', '_light');
    light.setRadius(5.0);
    light.setDiffuseColor(0.0, 1.0, 0.0);    
  end

  # setup the agents
  def createAgents()


    # add slow knot
    createAgent(@scene, 'slowknot', 'model/knot.void', 3.0, 4.0);

    # add fast knot
    createAgent(@scene, 'fastknot', 'model/knot.void', 3.0, 10.0);

    # add midspeed
    createAgent(@scene, 'mid', 'model/slow.void', 1.0, 6.0);

    # add turbospeed
    createAgent(@scene, 'turbo', 'model/slow.void', 1.0, 30.0);

    # add fast
    count = 1.0;
    4.times do
      createAgent(@scene, 'fast'+count.to_s, 'model/fast.void', 1.0, 10.0);
      count += 1;
    end

    # add food
    createFood();
  end
end

init = SurvivalInitializer.new;





