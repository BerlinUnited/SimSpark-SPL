# we place all the application data in the data/ directory,
# so set this up here

loadFromZip = false

if loadFromZip == true
	importBundle 'filesystemzip';
	fileServer = get ('/sys/server/file');
	fileServer.mount ('FileSystemZIP', 'data.zip');
else
	importBundle 'filesystemstd';
	fileServer = get ('/sys/server/file');
	fileServer.mount ('FileSystemSTD', 'data/');
end

importBundle ('inputsdl');
