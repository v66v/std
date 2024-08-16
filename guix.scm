(use-modules (guix)
             (guix build-system gnu)
             (guix git-download)
             (gnu packages check)
             (gnu packages fontutils)
             (gnu packages pkg-config)
             (gnu packages commencement)

             ((guix licenses)
              #:prefix license:)
             (gnu packages autotools))

(define vcs-file?
  ;; Return true if the given file is under version control.
  (or (git-predicate (current-source-directory))
      (const #t)))

(package
 (name "std")
 (version "0.0.1-git")
 (source (local-file "."
                     "std-checkout"
                     #:recursive? #t
                     #:select? vcs-file?))
 (build-system gnu-build-system)
 (arguments
  (list #:phases
        #~(modify-phases %standard-phases
                         (delete 'configure)
                         (delete 'build)
                         (delete 'check)
                         (replace 'install
                                  (lambda* (#:key outputs #:allow-other-keys)
                                    (let ((out (assoc-ref outputs "out")))
                                      (invoke "make" "install"
                                              (string-append "DESTDIR=" out))))))))
 (native-inputs (append (list libtool
                              cmocka
                              pkg-config
                              fontconfig)))
 (synopsis "A simple c standard library extension library.")
 (description "A simple c standard library extension library.")
 (home-page "https://github.com/v66v/std")
 (license license:lgpl3+))
