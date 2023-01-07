DISCONTINUATION OF PROJECT

This project will no longer be maintained by Intel.

Intel has ceased development and contributions including, but not limited to, maintenance, bug fixes, new releases, or updates, to this project.  

Intel no longer accepts patches to this project.

If you have an ongoing need to use this project, are interested in independently developing it, or would like to maintain patches for the open source software community, please create your own fork of this project.  

Contact: webadmin@linux.intel.com
# VAAPI Bypass
VAAPI bypass enables host to Intel Vision Product accelerator media acceleration .

## Overview
* VAAPI bypass suite of components include an IA host libVA backend and Intel Vision Product (e.g. Keembay) remote proxy application.
* On IA, it is known as VAAPI Shim backend driver that replaces existing INTEL GEN driver (iHD_drv_video.so).
* On a Intel Vision Product (e.g. Keembay) remote target, a proxy VAAPI Shim application (hddl_bypass_shim).
* The combination of these 2 component working in tandem allows media workload to be sent and executed by a remote accelerator via a preferred transport (e.g. Xlink PCIe).

## Getting Started
Please follow these instructions to build and use the driver.

## Prerequisites
* GStreamer framework 1.16
* LIBVA version 2.1
* [SafeStringLib](https://github.com/intel/safestringlib)
* XLink PCIe components
    * Follow the steps below to obtained XLink PCIe related components for IA host:
        1. Obtain the kmb-xlink-pcie-host-driver-dkms-xxxx.deb package that usually come alongside with the Yocto BKC release build artifactory.
        2. Install the package with steps below:
        ```
        # Remove previously installed packages with:
        $ sudo apt remove kmb-xlink-pcie-host-driver-dkms
        # Install the debian package with:
        $ sudo apt install kmb-xlink-pcie-host-driver-dkms-xxxx.deb
        ```
        3. Then, perform the commands below for the module installation and setup as per IA-host boot up:
        ```
        $ sudo modprobe mxlk
        $ sudo modprobe xlink
        $ sudo chmod 666 /dev/xlnk
        ```
* HDDLUnite (optional) -- Required only based on user's need.
    * Follow the steps below to obtain and build the HDDLUnite related components:
        1. Build and install HDDLUnite by following the instruction [here](https://gitlab.devtools.intel.com/kmb_hddl/hddlunite)
        2. The HDDLUnite supposed to be located in /opt/intel/hddlunite/, user have to set the follwing environment variable based on your file location:
        ```
        $ export KMB_INSTALL_DIR=/path/to/your/hddlunite/install/directory
        $ export LD_LIBRARY_PATH=${LD_LIBRARY_PATH }:/path/to/your/hddlunite/install/directory/lib
        ```

## Build instructions
1. Create a build directory in the vaapi_bypass source code:
   ```
   mkdir build
   ```
2. Export HDDLUnite environment variable (optional):
   ```
   # Export only when user want to use with HDDLUnite feature:
   $ export HDDLUNITE=1
   ```
3. Run cmake, and specify the option flag:
   ```
   # Build for IA:
   $ cmake -DTARGETS=IA -DDEBUG=<ON/OFF> -DUSE_HANTRO_DRIVER=<KMB> -DCMAKE_INSTALL_PREFIX=<path/to/install/dir> ..
   # Build for Keembay:
   $ cmake -DTARGETS=KMB -DDEBUG=<ON/OFF> -DCMAKE_INSTALL_PREFIX=<path/to/install/dir> ..
   ```
4. Compile it:
   ```
   make -j8
   ```
5. Install it:
   ```
   sudo make install
   ```

## Configuration for VAAPI Bypass Driver Communication Interface
* VAAPI Bypass driver supports two communication interfaces:
    * XLINK
    * HDDLUNITE

* User can setup the configuration in connection.cfg in the source code for IA Host side:
  ```
  MODE <XLINK/UNITE>
  CHANNELTX 0x401
  CHANNELRX 0x402
  ```
* For XLink mode, CHANNELTX and CHANNELRX pair on IA host and Keembay remote target should match. For example, if KMB set CHANNELTX (0x404) CHANNELRX (0x405) then IA side need to set CHANNELTX (0x405) CHANNELRX (0x404).

* Save the configuration file and set it as environment variable as:
  ```
  $ export CONFIG_PATH=/<path/to/connection.cfg>
  ```
* While on Keembay remote target side, hddl_bypass_shim binary provides run-time input for user to key in the required information such as mode, portTX, portRX for TCP mode; channelTX and channelRX for XLINK mode.


## How to get started with VAAPI Bypass driver
* After completing the setup, export the respective environment variable mentioned above on IA host and Keembay remote side:
  ```
  # export LIBVA_DRIVER_NAME in IA host.
  $ export LIBVA_DRIVER_NAME=hddl_bypass
  # Launch the hddl_bypass_shim apps on Keembay remote target.
  $ ./hddl_bypass_shim <tcp/xlink/unite>
  # Then, launch desired gstreamer command pipeline on IA host. For example:
  $ gst-launch-1.0 filesrc location=<input_file> ! h264parse ! vaapih264dec ! vaapijpegenc ! multifilesink location=<output_file_location> sync=True
  ```
