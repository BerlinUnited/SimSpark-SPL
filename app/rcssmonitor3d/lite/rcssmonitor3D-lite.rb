#
# init ruby script for rcssmonitor3d
#

# import the sexpparser parser plugin
importBundle "sexpparser"

$serverPath = '/sys/server/'

# setup the CommServer
new('rcssmonitor3d/CommServer', $serverPath+'comm')
## setup the LogfileServer instead of the CommServer for use as logplayer
#new('rcssmonitor3d/LogfileServer', $serverPath+'comm')
