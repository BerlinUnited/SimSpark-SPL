# here we perform the callback to zeitgeist
def method_missing (methId, *args)
	str = methId.id2name
	selectCall str, args
end

# set up some useful aliases
alias cd selectObject

# this method allows us to create new instance variables in class objects
# we use class objects as 'namespaces'
def createVariable (variable, value)
	# parse variable into a namespace and varName pair
	periodIndex = variable.index('.');
	if (periodIndex != nil && periodIndex > 0)
		nsName  = variable[0..(periodIndex-1)];
		varName = variable[(periodIndex+1)..-1];
		eval <<-EOS
			class #{nsName}
			end
			class << #{nsName}
				attr_accessor :#{varName}
			end
			#{nsName}.#{varName} = value
		EOS
	end
end

# this is a proxy class for objects created with 'new' via the Zeitgeist
# framework. It allows arbitrary methods to be called on the object and allows
# for much nicer syntax, than the select-call paradigm
class ZeitgeistObject
	# a pointer to the object represented by this object
	attr_reader :objPointer

	def method_missing (methId, *args)
		str = methId.id2name
		thisCall @objPointer, str, args
	end

	def initialize (objPointer)
		@objPointer = objPointer
	end
end
