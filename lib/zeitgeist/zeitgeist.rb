#
# this is the init script of the Zeitgeist library
# 

# here we perform the callback to zeitgeist
def method_missing(methId, *args)
	str = methId.id2name
	selectCall str, args
end

# set up some useful aliases
alias cd selectObject

# this method allows us to create a new class object, which we can misshandle
# as a namespace
def createNamespace (name)
  eval <<-EOS
    class #{name}
    end
  EOS
end

# this method allows us to create new instance variables in class objects
# we use class objects as 'namespaces'
def createVariable(classObj, variableName, value)
	eval <<-EOS
		class << #{classObj}
			attr_accessor :#{variableName}
		end
		#{classObj}.#{variableName} = value
	EOS
end

# this is a proxy class for objects created with 'create' via the Zeitgeist
# framework. It allows arbitrary methods to be called on the object and allows
# for much nicer syntax, than the select-call paradigm
class ZeitgeistObject
	# a pointer to the object represented by t
	attr_reader :objPointer

	def method_missing(methId, *args)
		#print "Missing method", @objPointer, "->", methId.id2name, "\n"
		str = methId.id2name
		thisCall @objPointer, str, args
	end

	def initialize(objPointer)
		#print "Creating new ZeitgeistObject ", objPointer, "\n"
		@objPointer = objPointer
	end
end
