def testFunction():
    print "Calling test export function."
    try:
        import soho
        print "Imported SOHO successfully."
    except:
        print "Can't import SOHO module."
