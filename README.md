    # Example
    
    RemoteValue<int> iHealth(address);

    while(true)
    {
        BaseRemote::ReadAll(pid);
        if(iHealth > 0) {
            ...
        }


        BaseRemote::WriteAll(pid);
    }
