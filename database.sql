CREATE TABLE `binds` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `bind` varchar(255) NOT NULL,
  `required_access` int(11) DEFAULT '0',
  `esql` text NOT NULL,
  PRIMARY KEY (`id`)
) DEFAULT CHARSET=utf8;

CREATE TABLE `perms` (
  `user` varchar(255) NOT NULL,
  `access` int(11) DEFAULT '0',
  PRIMARY KEY (`user`)
) DEFAULT CHARSET=utf8;
