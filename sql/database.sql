DROP TABLE IF EXISTS `binds`;
CREATE TABLE `binds` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `bind` varchar(255) NOT NULL,
  `required_access` int(11) DEFAULT '0',
  `esql` text NOT NULL,
  PRIMARY KEY (`id`)
) DEFAULT CHARSET=utf8;

DROP TABLE IF EXISTS `perms`;
CREATE TABLE `perms` (
  `user` varchar(255) NOT NULL,
  `access` int(11) DEFAULT '0',
  PRIMARY KEY (`user`)
) DEFAULT CHARSET=utf8;

DROP TABLE IF EXISTS `log`;
CREATE TABLE `log` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `timestamp` TIMESTAMP DEFAULT CURRENT_TIMESTAMP,
  `user` varchar(255) NOT NULL,
  `target` varchar(255) NOT NULL,
  `action` varchar(50) NOT NULL,
  `data` varchar(255) NOT NULL DEFAULT '',
  PRIMARY KEY (`id`)
) DEFAULT CHARSET=utf8;
