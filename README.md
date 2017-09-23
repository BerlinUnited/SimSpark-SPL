SimSpark-SPL
============

SimSpark simulator for Standard Platform Legaue

Improvements
------------

* NAO V4 Model
 * Realistic motor (stiffness, current, temperature)
 * Two cameras
 * Accurate mass model
 * Kinematics parameters
 * Image camera (optional)

* For Standard Platform League
 * Field, goal, ball
 * Game rules


Installation
------------

* Ubuntu

  ```sh
  sudo add-apt-repository ppa:xu-informatik/simspark-spl
  sudo apt-get update
  sudo apt-get install rcssserver3d-spl
  ```

  PPA: https://launchpad.net/~xu-informatik/+archive/simspark-spl

  If you have a newer Ubuntu version that is not yet supported by the PPA
  Then after you added the PPA, change the first line in /etc/apt/sources.list.d/xu-informatik-simspark-spl-trusty.list
  to:  ```http://ppa.launchpad.net/xu-informatik/simspark-spl/ubuntu <Version> main```

  Where ```<Version>``` refers to the adjective of your Ubuntu release.

  Then continue with the other two steps mentioned above.

* Windows

  download: https://dainas.dai-labor.de:5001/sharing/9s6N1iM2K
