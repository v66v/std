;;; Directory Local Variables            -*- no-byte-compile: t -*-
;;; For more information see (info "(emacs) Directory Variables")

((nil . ((user-full-name . "v66v")
         (user-mail-address . "74927439+v66v@users.noreply.github.com")
         (compile-command . "guix shell --pure -Df guix.scm -- make TYPE=debug")
         (eval . (progn (setq-local copyright-names-regexp
                                    (format "%s <%s>" user-full-name
                                            (string-replace "+" "\\+" user-mail-address)))
                        (setenv "ORGANIZATION" (format "%s <%s>"
                                                       user-full-name
                                                       user-mail-address)))))))
