#
# rcssmonitor3D-kerosin.rb
#

# import required plugins
importBundle 'sexpparser'

# setup the CommServer
new('rcssmonitor3d/CommServer', $serverPath+'comm')

# setup the MonitorParser
new('rcssmonitor3d/MonitorParser', $serverPath+'parser')

# let spark create a default camera
sparkAddFPSCamera(
		  $scenePath+'camera', 
		  x = 0, 
		  y = 5, 
		  z = 40,
		  maxSpeed = 15.0,
		  accel = 40.0,
		  drag = 4
		  )

#
# collection of callbacks and helper functions
#

# create a transform node and a node of class <className>
def addVisual(className, nodeName, x, y, z)
  transform = new('oxygen/Transform', $scenePath+nodeName)
  transform.setLocalPos(x,y,z)
  new('kerosin/'+className,$scenePath+nodeName+'/'+'visual')
end

def addSphere(nodeName, radius, material)
  transform = new('oxygen/Transform', $scenePath+nodeName)
  sphere = new('kerosin/Sphere',$scenePath+nodeName+'/visual')
  sphere.setRadius(radius)
  sphere.setMaterial(material)
end  

# called from the monitor to create an agent named <nodeName>
def addAgentN(nodeName)
  addSphere(nodeName,Soccer.AgentRadius,'matAgentN')
end

def addAgentL(nodeName)
  addSphere(nodeName,Soccer.AgentRadius,'matAgentL')
end

def addAgentR(nodeName)
  addSphere(nodeName,Soccer.AgentRadius,'matAgentR')
end

# called from the monitor to create a ball
def addBall(nodeName)
  addSphere(nodeName,Soccer.BallRadius,'matBall')
end

# called from the monitor to create a flag
def addFlag(nodeName)
  addSphere(nodeName,1.0,'matFlag')
end

# called from the monitor to create the playing field after the
# dimensions are received
def addField()
  # create the green
  fieldThickness = 0.5

  transform = new('oxygen/Transform', $scenePath+'field')
  transform.setLocalPos(0.0,-fieldThickness/2.0,0.0)

  field = new('kerosin/Box',$scenePath+'field/visual')
  field.setMaterial('matGround')

  field.setExtents(110.0,0.2,73.0)
end  

# create custom materials
material = new('kerosin/MaterialSolid', $serverPath+'material/matAgentN');
material.setColor(0.8,0.8,0.2)

material = new('kerosin/MaterialSolid', $serverPath+'material/matAgentL');
material.setColor(1.0,0.2,0.2)

material = new('kerosin/MaterialSolid', $serverPath+'material/matAgentR');
material.setColor(0.2,0.2,1.0)

material = new('kerosin/MaterialSolid', $serverPath+'material/matBall');
material.setColor(1.0,1.0,1.0)

material = new('kerosin/MaterialSolid', $serverPath+'material/matFlag');
material.setColor(1.0,0.0,0.0)

material = new('kerosin/MaterialSolid', $serverPath+'material/matGround');
material.setColor(0.1,0.5,0.1)

addVisual('Axis','myAxis',0.0,0.0,0.0)






