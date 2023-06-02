
/// A VIX host.
export class VixHost {
    /// Factory function for creating a VixHost which connects
    /// to VMware Workstation locally (only supported use case at the mopment)
    static ConnectLocal() : Promise<VixHost>;

    /// Open a VM on this VIX host.
    OpenVM(vmxPath: string) : Promise<VixVM>;
}

/// A VIX VM.
export class VixVM {
    //constructor(vmxpath : string);

    PowerOn() : Promise<void>;
    PowerOff() : Promise<void>;

    ReadVariable(
        type: number, // TODO: exported constants get ts types
        name: string
    ) : Promise<string>;
    
    WriteVariable(
        type: number,
        name: string,
        value: string
    ) : Promise<void>;

    /// Wait for VMware Tools to initalize on the guest (required to issue Tools functions).
    /// This function will eventually timeout and fail if Tools is not installed on the guest.
    WaitForTools() : Promise<void>;

    /// TOOLS FUNCTIONS (TODO)
}