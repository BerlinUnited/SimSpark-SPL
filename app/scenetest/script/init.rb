# we place all the application data in the data/ directory,
# so set this up here

loadFromZip = false

if loadFromZip == true
	importBundle 'bundle/filesystemzip';
	fileServer = get ('/sys/server/file');
	fileServer.mount ('FileSystemZIP', 'data.zip');
else
	importBundle 'bundle/filesystemstd';
	fileServer = get ('/sys/server/file');
	fileServer.mount ('FileSystemSTD', 'data/');
end

importBundle ('bundle/inputsdl');
