# here we perform the callback to zeitgeist
def method_missing(methId, *args)
	str = methId.id2name
	thisCall str, args
end

# set up some useful aliases
alias cd selectObject

# this method allows us to create new instance variables in class objects
# we use class objects as 'namespaces'
def createVariable classObj, variableName, value
  eval <<-EOS
    
    class << classObj
      attr_accessor :#{variableName}
    end
    classObj.#{variableName} = value
  EOS
end
