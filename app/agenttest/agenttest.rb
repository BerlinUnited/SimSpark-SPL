!#
# init ruby script for agenttest
#

# import the sexpparser parser plugin
importBundle "sexpparser"

# setup the CommServer and WorldModel
new('CommServer', '/sys/CommServer')
new('WorldModel', '/sys/WorldModel')

# install the 'kick and run' behavior
new('KickNRun', '/sys/Behavior')


